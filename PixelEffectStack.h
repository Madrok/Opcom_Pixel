#ifndef PIXELEFFECTSTACK_H_INCLUDED
#define PIXELEFFECTSTACK_H_INCLUDED

/**
  Linked list of effects
  */
struct PENode {
    /** effect pointer */
    PixelEffect *effect;
    /** pointer to next item in list */
    PENode *next;
};

/*
Though it seems simple, the following code FAILS

void effectStack() {
  PixelEffectStack stack = PixelEffectStack(&panel1);
  //stack.debug = debug;
  //stack.addEffect(new PixelEffect_Solid(&panel1, 0xFFFF00));
  stack.addEffect(new PixelEffect_KnightRider(&panel1,16,3));
  stack.init();

  while(true) {
    stack.run();
    Serial.println("Finished run");
    strip.show();
    Serial.println("Show complete");
    delay(100);
  }
}

For some reason, the Arduino resets right at "Show complete". Looks like a pointer or memory
error, but very unusual. I don't have a debugger environment, so someone who does more arduino
development could help out here.
*/

/**
  This is not currently implemented.
  Stack effects in a panel on top of each other, so that one draws on top of another.
  @todo Debug arduino to find out why only a few pixels are displayed when using the Knight Rider effect. See code in comments
*/
class PixelEffectStack : public PixelEffect {

private:
    /** list of effects */
    PENode *m_list;

public:
    //void (*debug)(char *);
    /**
      Create an effect stack for a strip
      @param strip Physical strip or logical Panel pointer
    */
    PixelEffectStack(PixelStrip *strip=NULL) :
        PixelEffect(strip), m_list(NULL)
    {
    };

    ~PixelEffectStack() {
        PENode *l;
        while(m_list) {
            l = m_list;
            if(m_list->effect) free(m_list->effect);
            free(m_list);
            m_list = l;
        }
    };

    /**
        Add an effect to the stack. The last effect added is run last, so it will
        draw on top of any earlier effects. Create PixelEffects with new() as this
        class will take ownership of the pe pointer.
        @param pe A PixelEffect pointer. Warning... these will be free()d by this class on destruction.
    */
    void addEffect(PixelEffect *pe) {
        //if(debug) debug("== Adding effect ==");
        PENode *n = new PENode;
        if(!n) {
            //if(debug) debug("Unable to create node");
            return;
        }
        //if(debug) debug("node created");
        n->effect = pe;
        n->next = NULL;

        if(!m_list) {
            m_list = n;
            //if(debug) debug("created list");
        } else {
            PENode *cur = m_list;
            while(cur->next != NULL) {
                if(cur->next == NULL) {
                    cur->next = n;
                }
                cur = cur->next;
            }
        }
        //if(debug) debug("--- end addEffect ----");
    }

    void init() {
        PENode *n = m_list;
        //if(n) if(debug) debug("initing list");
        while(n) {
            if(n->effect) {
                //if(debug) debug("initing effect");
                n->effect->init();
            }
            n = n->next;
        }
    }

    void run()
    {
        //if(debug) debug("==== Stack RUN ===\0");
        PENode *n = m_list;
        //n = m_list;
        //if(debug) {
        //  if(n) debug("have at least one effect\0");
        //  if(n->effect) debug("it is valid\0");
        //  else debug("it is NOT valid\0");
        //}
        while(n != NULL) {
            //if(debug) debug("Start effect\0");
            if(n->effect) n->effect->run();
            //if(debug) debug("Effect finished\0");
            n = n->next;
            //if(debug) debug("Got next pointer\0");
            //if(!n && debug) debug("...and it is null\0");
        }
    }
};
#endif // PIXELEFFECTSTACK_H_INCLUDED
