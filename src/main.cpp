#include <iostream>

#include <vector>
using namespace std;

void printResources(vector<int>available, vector<vector<int>>needed, vector<vector<int>>allocation)
{
 cout<<"Available resources:\n";
 char start='A';
 for(int i=0;i<available.size();i++){
    cout<<start<<" ";
    start++;
 }
 cout<<endl;
 for(int i=0;i<available.size();i++) cout<<available[i]<<" ";
 cout<<endl<<endl<<endl;

 cout<<"Needed resources:\n";
 start='A';
 for(int i=0;i<available.size();i++){
    cout<<start<<" ";
    start++;
 }
 cout<<endl;
 for(int i=0;i<needed.size();i++){
    for(int j=0;j<available.size();j++){
        cout<<needed[i][j]<<" ";
    }
    cout<<endl;
 }
 cout<<endl<<endl<<endl;
 cout<<"allocated resources:\n";
 start='A';
 for(int i=0;i<available.size();i++){
    cout<<start<<" ";
    start++;
 }
 cout<<endl;
 for(int i=0;i<allocation.size();i++){
    for(int j=0;j<available.size();j++){
        cout<<allocation[i][j]<<" ";
    }
    cout<<endl;
 }
  cout<<endl<<endl<<endl;
}

bool bankerAlogrithm(vector<int>available, vector<vector<int>>needed, vector<vector<int>>allocation,bool print) {
    pair<vector<int>, bool> result;

    int n = 5, m = 3, counter = 0;
    bool hasProcess = true;
    vector<int> done;
    bool isdone = false;

    while (true)//start of the alogrithm
    {
        int detect = 0;
        for (int i = 0; i < n; i++)
        { // processes

            for (vector<int>::iterator it = done.begin();
                it != done.end();
                it++)
            {
                if (*it == i)
                {
                    isdone = true;

                }
            }
            if (isdone == true) {
                isdone = false;
                continue;
            }


            for (int j = 0; j < m; j++)
            {                                    // resourses
                if (needed[i][j] > available[j]) // avilable is not enough
                {
                    hasProcess = false;
                    break;
                }
            }

            if (hasProcess == true) // got all it wants
            {
                detect++;//counter to know if any process got what it want in this loop
                counter++;//counter for how many proccesses has ended
                done.push_back(i);
                for (int j = 0; j < m; j++)
                {
                    available[j] += allocation[i][j];
                    allocation[i][j]=0;
                    needed[i][j]=0;
                }
                if(print==true){
                    cout<<endl<<"p"<<i<<" has done."<<endl;
                    printResources(available,needed,allocation);
                }
                i = -1;
            }
            hasProcess = true;


        }
        if (counter >= n)
        {
            if(print==true)cout<<"no Deadlock."<<endl;
            return true;
        }
        if (detect == 0) {
            if(print==true)cout<<"There is Deadlock!!."<<endl;
            return false;
        }
    }

}

void recover(vector<int>&available, vector<vector<int>>&needed, vector<vector<int>>&allocation) {
    int processNo = 0;
    int max = 0;
    int sum = 0;
    while (!bankerAlogrithm(available, needed, allocation,false))
    {
        for (int i = 0; i < needed.size(); i++)
        {
            for (int j = 0; j < available.size(); j++)
            {
                sum += allocation[i][j];
            }
            if (sum > max)
            {
                max = sum;
                processNo = i;
            }
            sum = 0;
        }
        for (int j = 0; j < available.size(); j++)
        {
            available[j] += allocation[processNo][j];
            needed[processNo][j] += allocation[processNo][j];
            allocation[processNo][j] = 0;
        }
        cout<<endl<<"P"<<processNo<<" has Released\n"<<endl;
        printResources(available,needed,allocation);
        bankerAlogrithm(available, needed, allocation,true);
    }

}

int main()
{
    int n = 5, m = 3;
    vector<int> available(m);
    vector<vector<int>> needed(n, vector<int>(m, 0));
    vector<vector<int>> maximum(n, vector<int>(m, 0));
    vector<vector<int>> allocation(n, vector<int>(m, 0));

    bool safe;


    cout << "Enter the initial number of available resources (space seperated):\n";
    for (int i = 0; i < m; i++)
    {
        cin >> available[i];
    }
    cout << "Enter the maximum need for each process in a seperate line:\n";
    for (int i = 0; i < n; i++)
    {

        for (int j = 0; j < m; j++)
        {
            cin >> maximum[i][j];
        }
    }
    cout << "Enter the allocated resources for each process in a seperate line:\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> allocation[i][j];
            available[j] -= allocation[i][j];
        }
    }

    //calculate needed
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            needed[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    safe = bankerAlogrithm(available, needed, allocation,true);

    //taking commands
    string command;
    int process, * resources = new int[m];
    while (true)
    {
        cout << "Enter command:\n";
        cin >> command;
        if (command == "Quit")
        {
            return 0; // terminate program
        }
        else if (command == "Recover")
        {
            if (!safe)
            {
                recover(available, needed, allocation);
                safe=true;
            }
            else
            {
                cout << "System already safe, no need to recover\n";
            }
            continue;
        }
        if(command!="RQ"||command!="RL"){cout<<"Invalid Command.!"<<endl;continue;}

        cin >> process;
        for (int i = 0; i < m; i++)
        {
            cin >> resources[i];
        }
        //check safe state
        if (!safe)
        {
            cout << "System has deadlock, please recover first\n";
            continue;
        }

        if (command == "RQ")
        {
            bool isValid = true;
            string message;
            // check request <= needed
            for (int i = 0; i < m; i++)
            {
                if (resources[i] > needed[process][i])
                {
                    isValid = false;
                    message = "ERROR: process has exceeded its maximum claim\n";
                }
            }
            //check request <= available
            for (int i = 0; i < m; i++)
            {
                if (resources[i] > available[i])
                {
                    isValid = false;
                    message = "ERROR: not enough resources available\n";
                }
            }
            if (!isValid)
            {
                cout << message;
                continue;
            }
            //request can be granted
            for (int i = 0; i < m; i++)
            {
                allocation[process][i] += resources[i];
                needed[process][i] -= resources[i];
                available[i] -= resources[i];
            }
            //execute safety algorithm
            safe = bankerAlogrithm(available, needed, allocation,true);
        }
        else if (command == "RL")
        {
            bool isValid = true;
            //check release <= allocation
            for (int i = 0; i < m; i++)
            {
                if (resources[i] > allocation[process][i])
                {
                    cout << "ERROR: resources exceeded allocation\n";
                    isValid = false;
                    break;
                }
            }
            if (!isValid)
            {
                continue;
            }
            // release granted
            for (int i = 0; i < m; i++)
            {
                available[i] += resources[i];
                needed[process][i] += resources[i];
                allocation[process][i] -= resources[i];
            }
            safe = bankerAlogrithm(available, needed, allocation,true);
        }
    }
}
