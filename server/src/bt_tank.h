#ifndef INCLUDED_BT_TANK
#define INCLUDED_BT_TANK

namespace bt {

class Tank {

public: 
    enum Status {
        READY, 
        ACTIVE, 
        DESTROYED
    };

    Tank(); 

private: 
    Status d_status;
};


class PlayerTank : public Tank {

public: 

};

}
#endif
