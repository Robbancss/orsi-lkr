#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <thread>
#include <future>

void nyomtat(std::vector<std::vector<std::pair<int, int>>>& b, std::string& oSample, int i, int j, std::string& result);

std::string lkr(std::string oSample, std::string compareSample)
{
    std::vector<std::vector<int> > c;
    std::vector<std::vector<std::pair<int, int>>> b;

    oSample = "." + oSample;
    compareSample = "-" + compareSample;

    int oSize = oSample.size();
    int cSize = compareSample.size();

    c.resize(oSize);
    b.resize(oSize);
    for (int i = 0; i < oSize; i++)
    {
        c[i].resize(cSize);
        b[i].resize(cSize);
    }
    
    for (int i = 0; i < oSize; i++)
    {
        c[i][0] = 0;
    }
    for (int j = 1; j < cSize; j++)
    {
        c[0][j] = 0;
    }
    for (int i = 1; i < oSize; i++)
    {
        for (int j = 1; j < cSize; j++)
        {
            if (oSample[i] == compareSample[j])
            {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = std::pair<int, int>(i-1, j-1);
            }
            else
            {
                if (c[i-1][j] >= c[i][j-1])
                {
                    c[i][j] = c[i-1][j];
                    b[i][j] = std::pair<int, int>(i-1, j);
                }
                else
                {
                    c[i][j] = c[i][j-1];
                    b[i][j] = std::pair<int, int>(i, j-1);
                }
            }
        }
    }

    std::string temp = "";
    nyomtat(b, oSample, oSize-1, cSize-1, temp);
    return temp;
}

void nyomtat(std::vector<std::vector<std::pair<int, int>>>& b, std::string& oSample, int i, int j, std::string& result)
{
    if (i == 0 || j == 0)
    {
        return;
    }
    if (b[i][j] == std::pair<int, int>(i-1, j-1))
    {
        nyomtat(b, oSample, i-1, j-1, result);
        // std::cout << oSample[i] << "-" << i; // itt kéne eltárolni
        result = result + oSample[i];
    }
    else
    {
        if (b[i][j] == std::pair<int, int>(i-1, j))
        {
            nyomtat(b, oSample, i-1, j, result);
        }
        else
        {
            nyomtat(b, oSample, i, j-1, result);
        }
        
    }
}

int main()
{
    std::vector<std::string> parsedInput;
    std::string originalSample;
    int comparedSampleCounter;
    std::ifstream ifs("input.txt");
    std::string temp;

    std::vector<std::future<std::string> > futureResults;

    {
        std::getline(ifs, temp);
        std::istringstream iss(temp);
        iss >> comparedSampleCounter;
        iss >> originalSample;
    }
    // std::cout << comparedSampleCounter << " " << originalSample << std::endl;

    while (std::getline(ifs, temp))
    {
        // std::istringstream iss(temp);
        parsedInput.push_back(temp);
    }

    for (int i = 0; i < comparedSampleCounter; i++)
    {
        futureResults.push_back(std::async(std::launch::async, lkr, originalSample, parsedInput[i]));
    }

    std::vector<std::string> outPutStrings;
    for (int i = 0; i < comparedSampleCounter; i++) {
        outPutStrings.push_back(futureResults[i].get());
    }
    

    std::ofstream out("output.txt");
    for (int i = 0; i < comparedSampleCounter; i++) {
        out << outPutStrings[i] << std::endl;
    }
    out.close();


    return 0;
}
