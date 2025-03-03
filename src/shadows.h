/* -*- C++ -*- */

#ifndef RSITE_ARDUINO_MENU_SHADOWS
  #define RSITE_ARDUINO_MENU_SHADOWS

  #include "menuBase.h"

  namespace Menu {

    struct promptShadowRaw {
      actionRaw a;//the hooked callback function
      systemStyles sysStyles;
      constText*text;
      constMEM eventMask events;//registered events
      styles style;
    };
    class promptShadow:public action {
      protected:
      public:
        systemStyles sysStyles;
        constText*text;
        constText **ptext=NULL; // multi-lang string array
        eventMask events;//registered events (mask)
        styles style;
      public:
        promptShadow(constText* t,action a=doNothing,eventMask e=noEvent,styles s=noStyle,systemStyles ss=_noStyle)
          :action(a),sysStyles(ss),text(t),events(e),style(s) {}
        promptShadow(constText* pt[],action a=doNothing,eventMask e=noEvent,styles s=noStyle,systemStyles ss=_noStyle,idx_t lang=0)
          :action(a),sysStyles(ss),ptext(pt),events(e),style(s) { text = pt[lang]; }
        inline void chLang(idx_t l) { if(ptext) text = ptext[l]; }
        inline constText* getText() const {return (constText*)memPtr(text);}
        inline systemStyles _sysStyles() const {return (systemStyles)memEnum(&sysStyles);}
        inline eventMask _events() const {return (eventMask)memEnum(&events);}
        inline styles _style() const {return (styles)memEnum(&style);}
    };
    struct textFieldShadowRaw {
      actionRaw a;
      systemStyles sysStyles;
      constText*text;
      eventMask events;//registered events
      styles style;
      char* buffer;
      char* const* validators;
      idx_t sz;
    };
    class textFieldShadow:public promptShadow {
      protected:
      public:
        char* buffer;
        char* const* validators;
        idx_t sz;
        textFieldShadow(
          constText* label,
          char* b,
          idx_t sz,
          char* const* v,
          action a=doNothing,
          eventMask e=noEvent,
          styles style=noStyle,
          systemStyles ss=(Menu::systemStyles)(_noStyle|_canNav|_parentDraw)
        ):promptShadow(label,a,e,style,ss),buffer(b),validators(v),sz(sz) {}
        textFieldShadow(
          constText** plabel,
          char* b,
          idx_t sz,
          char* const* v,
          action a=doNothing,
          eventMask e=noEvent,
          styles style=noStyle,
          systemStyles ss=(Menu::systemStyles)(_noStyle|_canNav|_parentDraw),
          idx_t lang=0
        ):promptShadow(plabel,a,e,style,ss,lang),buffer(b),validators(v),sz(sz) {}
        idx_t _sz() const {return (idx_t)memIdx(sz);}
        char* _buffer() const {return (char*)memPtr(buffer);}
        char* const* _validators() const {return (char* const*)memPtr(validators);}
        inline constText* operator[](idx_t i) const {
          return (char*)memPtr(((char**)_validators())[i]);
        }
    };
    struct menuNodeShadowRaw {
      actionRaw a;
      systemStyles sysStyles;
      constText*text;
      eventMask events;//registered events
      styles style;
      idx_t sz;
      prompt* constMEM* data;
    };
    class menuNodeShadow:public promptShadow {
      protected:
      public:
        idx_t sz;
        prompt* constMEM* data;
      public:
        menuNodeShadow(constText** ptext,idx_t sz,prompt* constMEM* data,action a,eventMask e,styles style,systemStyles ss=(systemStyles)(_menuData|_canNav),idx_t lang=0)
        :promptShadow(ptext,a,e,style,ss,lang),sz(sz),data(data) {}
        menuNodeShadow(constText* text,idx_t sz,prompt* constMEM* data,action a,eventMask e,styles style,systemStyles ss=(systemStyles)(_menuData|_canNav))
        :promptShadow(text,a,e,style,ss),sz(sz),data(data) {}
        idx_t _sz() const {return (idx_t)memIdx(sz);}
        prompt* constMEM* _data() const {return (prompt* constMEM*)memPtr(data);}
        inline prompt& operator[](idx_t i) const {
          return *(prompt*)memPtr(((prompt**)_data())[i]);
          //return *(prompt*)memPtr(((prompt**)memPtr(((menuNodeShadow*)shadow)->data))[i]);
        }
    };

    struct fieldBaseShadowRaw {
      actionRaw a;
      systemStyles sysStyles;
      constText*text;
      eventMask events;//registered events
      styles style;
      constText* units;
    };
    class fieldBaseShadow:public promptShadow {
      public:
        constText* units;
        fieldBaseShadow(constText** ptext,constText*units,action a=doNothing,eventMask e=noEvent,styles s=noStyle,systemStyles ss=((Menu::systemStyles)(Menu::_canNav|Menu::_parentDraw)),idx_t lang=0)
          :promptShadow(ptext,a,e,s,ss,lang),units(units) {}
        fieldBaseShadow(constText* text,constText*units,action a=doNothing,eventMask e=noEvent,styles s=noStyle,systemStyles ss=((Menu::systemStyles)(Menu::_canNav|Menu::_parentDraw)))
          :promptShadow(text,a,e,s,ss),units(units) {}
        inline constText* _units() {return (constText*)memPtr(units);}
    };
    template<typename T>
    struct menuFieldShadowRaw {
      actionRaw a;
      systemStyles sysStyles;
      constText*text;
      eventMask events;//registered events
      styles style;
      constText* units;
      T* value;
      constMEM T low,high,step,tune;
    };
    template<typename T>
    class menuFieldShadow:public fieldBaseShadow {
      protected:
      public:
        T* value;
        constMEM T low,high,step,tune;
      public:
        menuFieldShadow(T &value,constText* text,constText*units,T low,T high,T step,T tune,action a=doNothing,eventMask e=noEvent,styles s=noStyle,systemStyles ss=((Menu::systemStyles)(Menu::_canNav|Menu::_parentDraw)))
          :fieldBaseShadow(text,units,a,e,s,ss),value(&value),low(low),high(high),step(step),tune(tune) {}
        menuFieldShadow(T &value,constText** ptext,constText*units,T low,T high,T step,T tune,action a=doNothing,eventMask e=noEvent,styles s=noStyle,systemStyles ss=((Menu::systemStyles)(Menu::_canNav|Menu::_parentDraw)),idx_t lang=0)
          :fieldBaseShadow(ptext,units,a,e,s,ss,lang),value(&value),low(low),high(high),step(step),tune(tune) {}
        inline T& target() const {return *(T*)memPtr(value);}
        inline T getTypeValue(const T* from) const {
          //TODO: dynamic versions require change of preprocessor to virtual
          #ifdef USING_PGM
            T tmp;
            memcpy_P(&tmp, from, sizeof(T));
            return tmp;
          #else
            return *from;
          #endif
        }
        inline T _low() const {return getTypeValue(&low);}
        inline T _high() const {return getTypeValue(&high);}
        inline T _step() const {return getTypeValue(&step);}
        inline T _tune() const {return  getTypeValue(&tune);}
    };

    template<typename T>
    struct menuValueShadowRaw {
      actionRaw a;
      systemStyles sysStyles;
      constText*text;
      eventMask events;//registered events
      styles style;
      T value;
    };
    template<typename T>
    class menuValueShadow:public promptShadow {
      protected:
      public:
        T value;
      public:
        inline menuValueShadow(constText* text,T value,action a=doNothing,eventMask e=noEvent)
          :promptShadow(text,a,e),value(value) {}
        inline menuValueShadow(constText** ptext,T value,action a=doNothing,eventMask e=noEvent,idx_t lang=0)
          :promptShadow(ptext,a,e,lang),value(value) {}
        inline T getTypeValue(const T* from) const {
          //TODO: dynamic versions require change of preprocessor to virtual
          #ifdef USING_PGM
            T tmp;
            memcpy_P(&tmp, from, sizeof(T));
            return tmp;
          #else
            return *from;
          #endif
        }
        inline T target() const {return getTypeValue(&value);}
    };

    template<typename T>
    struct menuVariantShadowRaw {
      actionRaw a;
      systemStyles sysStyles;
      constText*text;
      constMEM eventMask events;//registered events
      styles style;
      idx_t sz;
      prompt* constMEM* data;
      //int width;//field or menu width
      //int ox,oy;//coordinate origin displacement
      T* value;
    };
    template<typename T>
    class menuVariantShadow:public menuNodeShadow {
      public:
        T* value;
        menuVariantShadow(
          constText* text,
          T &target,
          idx_t sz,
          prompt* constMEM* data,
          action a,
          eventMask e,
          styles style,
          systemStyles ss=(systemStyles)(_menuData|_canNav)
        ):menuNodeShadow(text,sz,data,a,e,style,ss),value(&target) {}
        menuVariantShadow(
          constText** ptext,
          T &target,
          idx_t sz,
          prompt* constMEM* data,
          action a,
          eventMask e,
          styles style,
          systemStyles ss=(systemStyles)(_menuData|_canNav),
          idx_t lang=0
        ):menuNodeShadow(ptext,sz,data,a,e,style,ss,lang),value(&target) {}
      inline T& target() const {return *((T*)memPtr(value));}
    };
  }//namespace Menu
#endif
