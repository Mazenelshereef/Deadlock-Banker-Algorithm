#include <iostream>
#include <vector>
using namespace std;

void printSequence(vector<int> processesSequence){
    for (int i = 0; i < processesSequence.size(); i++){
        cout << "Process " << processesSequence[i] << " has ended" << endl;
    }
}
void printStatus(vector<int> processesSequence, int num){
    if (processesSequence.size() < num){
        cout << "There is a possibility for a deadlock occurrence" << endl;
    }
    else{
        cout << "No deadlock" << endl;
    }
}

pair<vector<int>, bool> bankerAlogrithm(int numOfProcesses, int numOfResources, vector<int>available, vector<vector<int>>needed, vector<vector<int>>allocation) {
    pair<vector<int>, bool> result;
    int counter = 0;
    bool processGotWhatItWants = true;
    vector<int> done;
    bool isDone = false;

    while (true)
    {
        int detect = 0;
        for (int i = 0; i < numOfProcesses; i++){ 
            for (vector<int>::iterator iterator = done.begin(); iterator != done.end(); iterator++){
                if (*iterator == i){
                    isDone = true;
                }
            }
            if (isDone == true){ isDone = false; continue;} //ignore process which finished execution


            for (int j = 0; j < numOfResources; j++){                             
                if (needed[i][j] > available[j]) {    // avilable is not enough
                    processGotWhatItWants = false;
                    break;
                }
            }

            if (processGotWhatItWants == true) // got all it wants
            {
                detect++;//counter to know if any process got all resources it wants 
                counter++;//counter to check how many proccess has ended
                done.push_back(i);

                for (int j = 0; j < numOfResources; j++){
                    available[j] += allocation[i][j];
                }
                i = -1;
            }
            processGotWhatItWants = true;
        }

        if (counter >= numOfProcesses){
            result.first = done;
            result.second = true;
            return result;
        }
        if (detect == 0) {
            result.first = done;
            result.second = false;
            return result;
        }
    }

}

void recover(int NumOfProcess, int numOfResources, vector<int>available, vector<vector<int>>needed, vector<vector<int>>allocation) {
    int processNo = 0;
    int max = 0;
    int sum = 0;
    while (!bankerAlogrithm(NumOfProcess, numOfResources, available, needed, allocation).second){
        for (int i = 0; i < needed.size(); i++){
            for (int j = 0; j < available.size(); j++){
                sum += allocation[i][j];
            }
            if (sum > max){
                max = sum;
                processNo = i;
            }
            sum = 0;
        }
        for (int j = 0; j < available.size(); j++){
            available[j] += allocation[processNo][j];
            needed[processNo][j] += allocation[processNo][j];
            allocation[processNo][j] = 0;
        }
    }
}

int main()
{
    int numOfProcesses, numOfResources;
    bool isInSafeState;
    cout << "Enter the number of processes \n";
    cin >> numOfProcesses;
    cout << "Enter the number of resources \n";
    cin >> numOfResources;

    vector<int> available(numOfResources);
    vector<vector<int>> needed(numOfProcesses, vector<int>(numOfResources, 0));
    vector<vector<int>> maximum(numOfProcesses, vector<int>(numOfResources, 0));
    vector<vector<int>> allocation(numOfProcesses, vector<int>(numOfResources, 0));
    cout << "Enter the initial number of available resources (space seperated):\n";
    for (int i = 0; i < numOfResources; i++)
    {
        cin >> available[i];
    }
    cout << "Enter the maximum need for each process in a seperate line:\n";
    for (int i = 0; i < numOfProcesses; i++)
    {
        for (int j = 0; j < numOfResources; j++)
        {
            cin >> maximum[i][j];
        }
    }
    cout << "Enter the allocated resources for each process in a seperate line:\n";
    for (int i = 0; i < numOfProcesses; i++)
    {
        for (int j = 0; j < numOfResources; j++)
        {
            cin >> allocation[i][j];
            available[j] -= allocation[i][j];
        }
    }

    //calculate needed
    for (int i = 0; i < numOfProcesses; i++)
    {
        for (int j = 0; j < numOfResources; j++)
        {
            needed[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    isInSafeState = bankerAlogrithm(numOfProcesses, numOfResources, available, needed, allocation).second;
    printSequence(bankerAlogrithm(numOfProcesses, numOfResources, available, needed, allocation).first);
    printStatus(bankerAlogrithm(numOfProcesses, numOfResources, available, needed, allocation).first, numOfProcesses);

    //taking commands
    string command;
    int process, * resources = new int[numOfResources];
    while (true)
    {
        cout << "Enter command:\n";
        cin >> command;
        if (command == "Quit")
        {
            return 0; // terminate program
        }
        else if (command == "Recover"){
            if (!isInSafeState){
                recover(numOfProcesses, numOfResources, available, needed, allocation);
            }
            else{
                cout << "System already safe, no need to recover!\n";
            }
            continue;
        }

        //check safe state
        if (!isInSafeState){
            cout << "System has deadlock, please recover first!\n";
        }
        cin >> process;
        for (int i = 0; i < numOfResources; i++){
            cin >> resources[i];
        }
        if (command == "RQ"){
            bool isValid = true;
            string message;
            // check request <= needed
            for (int i = 0; i < numOfResources; i++){
                if (resources[i] > needed[process][i]){
                    isValid = false;
                    message = "ERROR: Process has exceeded its maximum claim!\n";
                }
            }
            //check request <= available
            for (int i = 0; i < numOfResources; i++){
                if (resources[i] > available[i])
                {
                    isValid = false;
                    message = "ERROR: Not enough resources available!\n";
                }
            }
            if (!isValid){
                cout << message;
                continue;
            }
            //request can be granted
            for (int i = 0; i < numOfResources; i++){
                allocation[process][i] += resources[i];
                needed[process][i] -= resources[i];
                available[i] -= resources[i];
            }
            //execute safety algorithm
            isInSafeState = bankerAlogrithm(numOfProcesses, numOfResources, available, needed, allocation).second;
        }
        else if (command == "RL"){
            bool isValid = true;
            //check release <= allocation
            for (int i = 0; i < numOfResources; i++)
            {
                if (resources[i] > allocation[process][i])
                {
                    cout << "ERROR: resources exceeded allocation!\n";
                    isValid = false;
                    break;
                }
            }
            if (!isValid){
                continue;
            }
            // release granted
            for (int i = 0; i < numOfResources; i++){
                available[i] += resources[i];
                needed[process][i] += resources[i];
                allocation[process][i] -= resources[i];
            }
            isInSafeState = bankerAlogrithm(numOfProcesses, numOfResources, available, needed, allocation).second;
        }
        else if (command == "Quit") {
            cout << "The system is exited.. " << endl;
            return 0;
        }
        else{
            cout << "INVALID COMMAND!\n";
        }
    }
}