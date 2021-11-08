//---------------------------------------------------------------------------
//sTGC pentagon map class
//Author:Zhen Wang, wangzhen@rcf.rhic.bnl.gov
//map data from Yingying Shi
//---------------------------------------------------------------------------
// 2021.10.14
//if the map updated, just need to update the input map file with same format
//Row_num    FEB_num    VMM_num    VMM_ch    strip_ch


#include "Strip2CH.h"

#include <map>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdio.h>
// #include <string>

using namespace std;


//initialize the relationship between the electronic channels and the shirps
//the VMM channel will be named as #FEB*10000+#Vmm*1000+Vmm_channel
//Note the frist number of FEB and VMM is 0. if DAQ start at 0, the DAQ returned number should add 1
//in the map file the VMM channel start form 0.
// filename should be the path to save the data file 
void Strip2CH :: init( string path )
{
    ifstream inFile;

    // read the map of pentagon information 
    string filename = path+"/stationMap.txt";
    inFile.open( filename );
    string line;
    string FirstWord = "Row_num";
    int Row_num = 0, FEB_num = 0, VMM_num = 0, VMM_ch  = 0,strip_ch = 0;
    char First_word[512];
    int nLines = 0;
    while (getline(inFile, line))
    {   
        istringstream iss(line);
        if ( (int)line.at(0) < 48 || (int)line.at(0) > 57 ) continue;
        cout << line << endl;
        iss >> Row_num >> FEB_num >> VMM_num >> VMM_ch >> strip_ch;
        cout << "Row_num = " << Row_num << " FEB_num = " << FEB_num << " Vmm_num = " << VMM_num << " Vmm_ch = " << VMM_ch << " Strip channel = " << strip_ch << endl;
        int VMM_number = FEB_num*10000+VMM_num*1000+VMM_ch;
        int Strip_number = (Row_num+1)*1000+strip_ch;// if row start from 0, but I think it is under our control
        Channel_2_Strip[VMM_number] = Strip_number;
        cout << "VMM number = " << VMM_number << endl;
        cout << "Strip number = " << Strip_number << endl;
        nLines++;
    }
    cout << "totally input " << nLines << " strips" << endl;
    inFile.close();
    
    //read the Z position of each disk
    //using the data form yingying's slides https://indico.inp.nsk.su/event/20/contributions/970/attachments/555/640/INSTR20_poster_Yingying_Shi-v2.pdf
    filename = path+"/Disk_Z.txt";
    int Z_posi;
    inFile.open( filename );
    int nDisk = 1;
    while (inFile >> Z_posi)
    {
        Zpostion[nDisk] = Z_posi;
        nDisk++;
    }
    cout << "# total disk = " << nDisk-1 << endl;
    inFile.close();

    //station map from Prashanth's file https://drupal.star.bnl.gov/STAR/system/files/sTGC_Cable_Mapping_ver1.pdf
    filename = path+"/station_ROB.txt";
    inFile.open( filename );
    int Station_num = 0, ROB_num = 0;
    int nStation = 1;
    while (inFile >> Station_num >> ROB_num )
    {
        station[Station_num] = ROB_num;
        nStation++;
    }
    cout << "# total station = " << nStation-1 << endl;
    inFile.close();

    
}

// VMM channels were named as #FEB*10000+#Vmm*1000+Vmm_channel
// strips were named as #Row*1000+strip_number
int Strip2CH :: GetStripNumber( int FEB, int VMM, int Channel )
{
    FEB = FEB%6;//if FEBs are [0..5]
    int VMM_number = (FEB+1)*10000+(VMM+1)*1000+(Channel+1);//if VMM is [0..3]
    // FEB = FEB%6;//if FEBs are 1-96
    // if (FEB == 0) FEB = 6;//if FEBs are 1-96
    // int VMM_number = FEB*10000+VMM*1000+(Channel+1);//if VMM is [1..4]
    int strip_number = Channel_2_Strip[VMM_number]; // how about the fake VMM_number? how to debug? // the fake number will return 0
    cout << "VMM_number = " << VMM_number << " strip_number = " <<strip_number << endl;
    return strip_number;
}

//get the disk, quadrant, and station information
//#disk start from 1
//Quadrant arrange like this: 
// D A
// C B
// 1st is A, 2nd is D, 3rd is C, 4th is D
int Strip2CH :: GetStation(int ROB, int &Quadrant, int &Disk)
{
    Disk = (ROB-1)/4+1;
    Quadrant = (ROB-1)%4+1;
    int station_num = station[ROB];
    return station_num;
}
// get the front and back chamber
// if Real data start from 0, all the FEBs should add 1
// for the return number, 0 is front chamber, 1 is back chamber, return -1 means you put a wrong number
int Strip2CH :: GetChamber(int FEB)
{
    if ( FEB >= 0 && FEB <=5 )
        return (FEB)%2;
    else return -1;
}

