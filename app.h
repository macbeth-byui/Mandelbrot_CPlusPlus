#ifndef app_h
#define app_h

class App
{
    protected:
        bool refreshed;
    public:
        virtual void drawApp() = 0;
        virtual void calculateApp() = 0;
        bool isRefreshed() const { return refreshed; }
};

#endif
