#include "util.hpp"

void bigFlush(int* array, int size)
{


    for (int i = 0; i < size; i++) // FLUSH IT ALL!
    {
        array[i] += 1; 
        CLFLUSH(reinterpret_cast<ADDR_PTR>(&array[i]));
    }
}

// Pulse COUNT times
void pulse(int count)
{

    int intArray[2048];

    for (int i = 0; i < 2048; i++) 
    {
        intArray[i] = i;
    }

    for (int i = 0; i < count; i ++)
    {
        bigFlush(intArray, 2048);
    }
}


std::string zEncode(std::string binmsg)
{ // INPUT BETTER START WITH ONES!
    bool zmode = false;
    std::string zeroEncode = "";
    for (char c : binmsg)
    {
        if (!zmode and c == '1') zeroEncode += '1';
        else if (!zmode and c == '0') {zmode = true; zeroEncode += "01";}
        else if (zmode and c == '0') zeroEncode +='1';
        else if (zmode and c == '1') {zmode = false; zeroEncode +="01";}
    }
    return zeroEncode;
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
    char msg[125];
    std::cout << "Enter a message to transmit: ";

    fgets(msg, 125, stdin);

    std::string binmsg = "10111111"; //  indicates the start of a message. ?
    
    binmsg += convert_to_binary(msg);




    std::string encoded = zEncode(binmsg);

    cout << "Plaintext: " << binmsg;
    cout << "\n-0Encoded:" << encoded;
    cout << "Test Decode" << zDecode(encoded);

    int msgIndex = 0;

    std::cout << "\nBegin transmission sync...\n" << endl << flush;

    struct timespec start, end;
    
    while(true)
    {
    clock_gettime(CLOCK_MONOTONIC, &start);
    if (start.tv_sec % 5 == 0) // Try to start the clock every 5 human seconds.
        {
            std::cout << "Synced! Transmitting...\n";
            break;
        }   
    }
    

    clock_gettime(CLOCK_MONOTONIC, &start);

    long long diff;
    bool val = true;
    while(true) {
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = end.tv_sec - start.tv_sec;
        if (diff > 2)
        {
            val = (encoded[msgIndex] == '1');
            cout << val << flush;
            msgIndex += 1;
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
        if (val) pulse(10000);
        else usleep(10000);
    }








    exit(0);

}





    // Baesd on this experiment I can run a loop once every 100 cache cycles. It will change depending on loop contents though. I'll try to build a clocking cycle of 10,000.
    
    /*
    for (int i = 0; i < 10; i++)
    {
        list[i] = RDTSC();
    }
    for (int i = 1; i < 10; i++)
    {
        std::cout << list[i] - list[i - 1] << ", ";
    }
    */

