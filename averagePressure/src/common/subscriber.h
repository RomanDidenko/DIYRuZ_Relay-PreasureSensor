#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

typedef void (*SubscribeAction)();

class Subscriber
{
public:
    Subscriber(SubscribeAction subscribeAction)
    {
        subscribeAction();
        //delete this;
    };
};

#endif /* SUBSCRIBER_H */