# DiamondToOsc
Converts Diamond Apparatus topics to Open Sound Control messages. For more information on Diamond Apparatus, see https://github.com/jimfinnis/DiamondApparatus/.

## Usage
```
  diamond2osc <topicprefix> <port>
```
If the program detects a change on any topic starting with *topicprefix* it will rebroadcast the message (assumed to be an array of floats)
to the given port number (which should be 57120 for SuperCollider).

Internally, it subscribes to *topics* (so it will receive a list of topics when new ones are published). When
a new topic arrives, it will be subscribed to if the prefix matches. If a changed message arrives on such a topic,
it is retransmitted.
