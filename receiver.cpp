#include "util.hpp"

#define recentAvg 10 // Determines the short term rolling average for finding spikes
// higher number = more noise resistance?

#define swingCount 10 // Determines how many swings to use to calculate swing frequency

void init()
{
    int data = 1337;
    ADDR_PTR cacheAddr = reinterpret_cast<ADDR_PTR>(&data);

    std::cout << "\nInitializing...\n";
    CYCLES cachedClock = measure_one_block_access_time(cacheAddr);
    std::cout << "Time to access data in cache: " << cachedClock << std::endl;
    CLFLUSH(cacheAddr);
    cachedClock = measure_one_block_access_time(cacheAddr);
    std::cout << "\nTime to access data after flush: " << cachedClock << std::endl;

}


void tick()
{
    std::cout << "ticking..." << endl;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    while(true)
    {
        clock_gettime(CLOCK_MONOTONIC, &end);
        usleep(1000);
        if (end.tv_sec - start.tv_sec >= 1)
        {
            return;
        }
    }
}


std::string zDecode(std::string input)
{
    bool zmode = false;
    std::string plain = "";
    int len = input.length();
    for (int i = 0; i < len; i++)
    {
        if (!zmode and input[i] == '1') plain += '1';
        else if (!zmode and input[i] == '0') zmode = true;
        else if (zmode and input[i] == '1') plain += '0';
        else if (zmode and input[i] == '0') {plain += '1'; zmode = false; i += 1;}
    }

    return plain;


}


int main()
{


    ADDR_PTR cacheAddr = find_addr();

    CYCLES cachedClock;
    CYCLES lastN[recentAvg]; 
    int i = 0;
    CYCLES ROLLED = measure_one_block_access_time(cacheAddr);
    int n = 1;
    CYCLES lastAvg = 0;

    
    int cacheChecks = 0;  
    int swingIndex = 0;

    //int swingMilestone = 0;
    //int swingDensity;

    // Start the clock
    struct timespec start, end;
    while(true)
    {
    clock_gettime(CLOCK_MONOTONIC, &start);
    if (start.tv_sec % 5 == 0) // Try to start the clock every 5 human seconds.
        {
            std::cout << "Synced!\n";
            break; // Clock is now synced?
        }   
    }


    long long diff;

    clock_gettime(CLOCK_MONOTONIC, &start);

    std::string received = "";
    std::string decoded;
    bool receptionBit;
    bool startReading = false;

    while(true)
    {
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = end.tv_sec - start.tv_sec;
        if (diff > 2)
        {
            //std::cout << (swingIndex < 300) << flush; // High swings = 0
            receptionBit = (swingIndex > 500);
            swingIndex = 0;
            if (receptionBit and !startReading)
            {
                cout << "Reception starting!\n";
                startReading = true;
            } else if (startReading) {

            std::cout << receptionBit << flush;
            if (receptionBit) received += '1';
            else received += '0';

            decoded = zDecode(received);
            if (decoded.length() % 8 == 0) cout << endl << convert_from_binary(decoded.c_str(), decoded.length()) << endl;
            

            }

            clock_gettime(CLOCK_MONOTONIC, &start);
        }


        cachedClock = measure_one_block_access_time(cacheAddr);
        if (cachedClock > 3000) continue; // Sometimes get ridiculous values, not sure why, but this gets rid of them
        lastN[i] = cachedClock;

        n += 1; // Tracks index of while loop, until a swing is detected.
        cacheChecks += 1; // Tracks the index of the while loop. 


        // CALCULATE RECENT AVERAGE

        lastAvg = 0; // Reset sum for calculating the recent rolling average
        i = (i + 1) % recentAvg; // Change our recent rolling average index 
        for (int item = 0; item < recentAvg; item++)  // sum up our last average
        {
            lastAvg += lastN[item];
        }
        lastAvg = lastAvg / 10;


        //Calculate rolling average (Since last swing);

        ROLLED = (ROLLED * (n - 1) + cachedClock) / n;

        //std::cout << "\n" << cachedClock << ":";
        //std::cout << "Rolling:" << ROLLED << " lastN:" << lastAvg << " ";


        if (abs(static_cast<int>(lastAvg - ROLLED)) > (ROLLED / 10) * 3) // Detects a change of 30%
        {
            //std::cout << " Swing of " << lastAvg - ROLLED;
            ROLLED = lastAvg;
            n = recentAvg;

            swingIndex += 1;
            // if (swingIndex > swingCount)
            // {
            //     swingDensity = cacheChecks - swingMilestone; // lower swing density = more swings
            //     swingMilestone = cacheChecks;
            //     swingIndex = 0;
            //     std::cout << "Density: " << swingDensity << "\n";
            // }
        }




        usleep(100);

    }


    exit(0);
}
// std::cout << i << "Incoming message? " << cachedClock << std::endl;
