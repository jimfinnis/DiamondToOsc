/**
 * @file main.cpp
 * @brief  Brief description of file.
 *
 */

#include <set>
#include <string>
#include <unistd.h>
#include <lo/lo.h>
#include <diamondapparatus/diamondapparatus.h>

using namespace diamondapparatus;

static std::set<std::string> myTopics;

static void handler(int sig){
    printf("sig %d caught\n",sig);
    destroy(); // shut down client
    exit(1); // no safe way to continue
}

// this processes a list of topics (itself in a topic)
// and subscribes to those which are of interest

static void handleTopics(Topic& list,const char *prefix){
    for(int i=0;i<list.size();i++){
        if(!strncmp(prefix,list[i].s(),strlen(prefix))){
            const char *t = list[i].s();
            if(myTopics.find(t) == myTopics.end()){
                subscribe(t);
                myTopics.insert(t);
                printf("New topic: %s\n",t);
            }
        }
    }
}

static lo_address lo;
static void sendOsc(const char *s, Topic &t){
    lo_message msg = lo_message_new();
    for(int i=0;i<t.size();i++)
        lo_message_add_float(msg,t[i].f());
    int ret = lo_send_message(lo,s,msg);
    lo_message_free(msg);
    
}

int main(int argc,char *argv[]){
    // the only argument is the prefix for the topics
    // we wish to rebroadcast to OSC
    
    if(argc<2){
        fprintf(stderr,"Usage: diamond2osc <topicprefix>\n");
        exit(1);
    }
    const char *prefix = argv[1];
    
    lo = lo_address_new(NULL,NULL);
    
    try {
        init();
        subscribe("topics");
        for(;;){
            waitForAny(); // wait for any message
            Topic t;
            t = get("topics");
            if(t.state == TOPIC_CHANGED)
                handleTopics(t,prefix);
            
            for(auto it = myTopics.begin();it!=myTopics.end();++it){
                const char *name = (*it).c_str();
                t = get(name);
                if(t.state == TOPIC_CHANGED){
                    sendOsc(name,t);
                }
            }

        }
    } catch(DiamondException e){
        fprintf(stderr,"Failed: %s\n",e.what());
        destroy();
        exit(1);
    }
        
}
