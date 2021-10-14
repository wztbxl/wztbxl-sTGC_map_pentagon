
// VMM channels were named as #FEB*10000+#Vmm*1000+Vmm_channel
// strips were named as #Row*1000+strip_number

#ifndef STRIP2CH_H
#define STRIP2CH_H

#include <map>
#include <string>

class Strip2CH
{
private:
    std :: map< int , int > Channel_2_Strip;
    std :: map< int , double > Zpostion;
    std :: map< int , double > Frist_Strip_Position;// unit: cm 
    std :: map< int , int > station;// unit: cm 
public:
    Strip2CH();
    void init( std :: string path ); // init the relationship between the electronic channels and the strips
    int GetStripNumber( int FEB, int VMM, int Channel );// update the rule for the pentagon map 
    int GetStation(int ROB, int &Quadrant, int &Disk);
    ~Strip2CH();
};

Strip2CH::Strip2CH()
{
}

Strip2CH::~Strip2CH()
{
}


#endif