#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdlib>

using namespace std;

const string ClientsFile = "Clients.txt";

void ShowMainMenu();

void ShowTransactionsScreen();

enum enMainScreenMenu { enShowClientList = 1, enAddClient = 2, enDeleteClient = 3, enUpdateClient = 4, enFindClient = 5, 
	enTransactions = 6, enExit = 7 };

enum enTranscationsScreenMenu { enDeposit = 1, enWithdraw = 2, enTotalBalances = 3, enMainMenu = 4};

struct strClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

vector <string> SplitString(string S1, string Delim) {

	vector <string> vString;
	short pos = 0;
	string sWord = "";

	while ((pos = S1.find(Delim)) != string::npos) {
		sWord = S1.substr(0, pos);

		if (sWord != "")
			vString.push_back(sWord);

		S1.erase(0, pos + Delim.length());
	}
	if (S1 != "")
		vString.push_back(S1);

	return vString;
}

strClient ConvertLineToRecord(string Line, string Seperator = "#//#") {
	strClient Client;
	vector <string> vClient = SplitString(Line, Seperator);

	Client.AccountNumber = vClient[0];
	Client.PinCode = vClient[1];
	Client.Name = vClient[2];
	Client.Phone = vClient[3];
	Client.AccountBalance = stod(vClient[4]);

	return Client;
}

string ConvertRecordToLine(strClient Client, string Seperator = "#//#") {

	string Line = "";

	Line += Client.AccountNumber + Seperator;
	Line += Client.PinCode + Seperator;
	Line += Client.Name + Seperator;
	Line += Client.Phone + Seperator;
	Line += to_string(Client.AccountBalance);

	return Line;
}

vector <strClient> LoadClientsDataFromFile(string FileName) {

	vector <strClient> vClients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;

		while (getline(MyFile, Line)) {

			if (Line != "")
				vClients.push_back(ConvertLineToRecord(Line));
		}

		MyFile.close();
	}
	return vClients;
}

void PrintClientRecord(strClient Client) {

	cout << "| " << left << setw(20) << Client.AccountNumber;
	cout << "| " << left << setw(15) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(15) << Client.Phone;
	cout << "| " << left << setw(10) << Client.AccountBalance;

}

void PrintClientBalance(strClient Client) {

	cout << "| " << left << setw(20) << Client.AccountNumber;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(10) << Client.AccountBalance;

}

void ShowAllClientsScreen() {

	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFile);

	cout << "\t\t\t\t\tClients List (" << vClients.size() << ") Clients.";
	cout << "\n____________________________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(20) << "Account Number";
	cout << "| " << left << setw(15) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(10) << "Balance";
	cout << "\n____________________________________________________________________________________________________________________\n\n";

	if (vClients.size() == 0) {

		cout << "\n\t\t\t\t\tNo Clients Available In the System!\n";
	}
	else {
		for (strClient& N : vClients) {

			PrintClientRecord(N);
			cout << endl;

		}
	}
	cout << "____________________________________________________________________________________________________________________\n";
}

void ShowAllClientsBalancesScreen() {

	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFile);

	cout << "\t\t\t\t\tBalances List (" << vClients.size() << ") Clients.";
	cout << "\n____________________________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(20) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(10) << "Balance";
	cout << "\n____________________________________________________________________________________________________________________\n\n";
	double TotalBalances = 0;
	if (vClients.size() == 0) {

		cout << "\n\t\t\t\t\tNo Clients Available In the System!\n";
	}
	else {
		for (strClient& N : vClients) {
			TotalBalances += N.AccountBalance;
			PrintClientBalance(N);
			cout << endl;

		}
	}
	cout << "____________________________________________________________________________________________________________________\n";
	cout << "\t\t\t\t\tTotal Balances = " << TotalBalances << "\n\n";
}

bool FindClientByAccountNumber(string AccountNumber, strClient& Client, vector <strClient> vClients) {

	for (strClient& C : vClients) {
		if (C.AccountNumber == AccountNumber) {
			Client = C;
			return true;
		}
	}
	return false;
}

bool isClientExistByAccountNumber(string AccountNumber, string ClientsFileName) {

	fstream MyFile;
	strClient Client;

	MyFile.open(ClientsFileName, ios::in);

	if (MyFile.is_open()) {
		string Line;
		while (getline(MyFile, Line)) {

			if (Line != "") {
				Client = ConvertLineToRecord(Line);
				if (AccountNumber == Client.AccountNumber) {
					MyFile.close();
					return true;
				}
			}

		}
		MyFile.close();
	}
	return false;
}

strClient ReadNewClient() {

	strClient Client;

	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	while (isClientExistByAccountNumber(Client.AccountNumber, ClientsFile)) {
		cout << "\nClient With [" << Client.AccountNumber << "] already Exists, Enter another Account Number ? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

void AddDataLineToFile(string FileName, string Client) {

	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open()) {

		MyFile << Client << endl;

		MyFile.close();
	}
}

void AddNewClient() {

	strClient stClient;
	stClient = ReadNewClient();
	AddDataLineToFile(ClientsFile, ConvertRecordToLine(stClient));

}

void AddNewClients() {

	char AddMore = 'y';

	do {

		cout << "Adding New Client: \n\n";

		AddNewClient();

		cout << "Client Added Successfully, do you want to add more clients? \n";

		cin >> AddMore;


	} while (tolower(AddMore) == 'y');
}

short ReadMainMenuOption() {

	short Choice;
	cout << "Choose What do you want to do? [1 to 7]? ";
	cin >> Choice;

	return Choice;
}

void ResetScreen() {
	system("cls");
}

void PrintClientCard(strClient Client) {

	cout << "\n\nThe following are the details:\n";
	cout << "-------------------------------------------\n";
	cout << "Account Number  : " << Client.AccountNumber << endl;
	cout << "Pin Code        : " << Client.PinCode << endl;
	cout << "Name            : " << Client.Name << endl;
	cout << "Phone           : " << Client.Phone << endl;
	cout << "Account Balance : " << Client.AccountBalance << endl;
	cout << "-------------------------------------------\n";
}

bool MarkForDeleteByAccountNumber(string AccountNumber, vector <strClient>& vClients) {
	for (strClient& C : vClients) {
		if (C.AccountNumber == AccountNumber) {
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

void SaveClientsToFile(string FileName, vector <strClient> vClients) {

	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open()) {
		for (strClient C : vClients) {
			if (C.MarkForDelete == false) {
				MyFile << ConvertRecordToLine(C) << endl;
			}
		}
		MyFile.close();
	}

}

string ReadClientNumber() {
	string AccountNumber;

	cout << "Please Enter Account Number? ";
	getline(cin >> ws, AccountNumber);

	return AccountNumber;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <strClient>& vClients) {
	char Answer = 'n';
	strClient Client;

	if (FindClientByAccountNumber(AccountNumber, Client, vClients)) {

		PrintClientCard(Client);
		cout << "Are You Sure you want to delete this client? y/n? ";
		cin >> Answer;

		if (tolower(Answer) == 'y') {

			MarkForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsToFile(ClientsFile, vClients);

			// Refresh Clients Vector
			vClients = LoadClientsDataFromFile(ClientsFile);
		}
		return true;
	}
	else {
		cout << "\nClient With Account Number (" << AccountNumber << ") is Not Found!\n";
		return false;
	}

}

strClient ChangeClientRecord(string AccountNumber) {

	strClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "Enter Pin Code? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <strClient>& vClients) {

	strClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, Client, vClients)) {

		PrintClientCard(Client);

		cout << "\n\nAre You Sure you want to delete this client? y/n? ";
		cin >> Answer;
		if (tolower(Answer) == 'y') {

			for (strClient& C : vClients) {
				if (C.AccountNumber == AccountNumber) {
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsToFile(ClientsFile, vClients);
			cout << "Client Deleted Successfully.";
			return true;
		}

	}
	else {
		cout << "\nClient With Account number (" << AccountNumber << ") Not Found!";
	}
	return false;

}

void EndScreen() {

	cout << "--------------------------------\n";
	cout << "\tProgram Ends\n";
	cout << "--------------------------------\n";

}

void GoBackToMainMenu() {
	cout << "\n\n\nPress Any Key To Go Back To Main Menu ........";
	system("pause>0");
	system("cls");
	ShowMainMenu();
}

void ShowAddNewClientsScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Client Screen";
	cout << "\n-----------------------------------\n";

	AddNewClients();
}

void ShowDeleteScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client  Screen";
	cout << "\n-----------------------------------\n";
	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFile);
	string AccountNumber = ReadClientNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n-----------------------------------\n";

	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFile);
	string AccountNumber = ReadClientNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";

	strClient Client;
	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFile);
	string AccountNumber = ReadClientNumber();
	if (FindClientByAccountNumber(AccountNumber, Client, vClients)) {
		PrintClientCard(Client);
	}
	else {
		cout << "\nThe Client with Account Number (" << AccountNumber << ") Not Found!\n";
	}
}

short ReadTransactionMenuOption() {

	short Choice;
	cout << "Choose What do you want to do? [1 to 4]? ";
	cin >> Choice;

	return Choice;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <strClient> &vClients) {

	char Answer = 'n';

	

	cout << "\nAre you sure you want to perform this transaction? y/n? ";
	cin >> Answer;

	if (tolower(Answer) == 'y') {
		for (strClient& C : vClients) {

			if (C.AccountNumber == AccountNumber) {
				C.AccountBalance += Amount;
				cout << "\nDone Successfully New Balance is " << C.AccountBalance << "\n\n";
				SaveClientsToFile(ClientsFile, vClients);
				return true;
			}

		}
	}
	return false;
}

void ShowDepositScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n\n";

	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFile);
	string AccountNumber = ReadClientNumber();
	strClient Client;

	while (!FindClientByAccountNumber(AccountNumber, Client, vClients)) {
		cout << "\nThe Client with (" << AccountNumber << ") Not Exist!\n";
		AccountNumber = ReadClientNumber();
	}
	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease Enter Deposit Balance? ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber,Amount, vClients);

}

void ShowWithdrawScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n-----------------------------------\n";

	vector <strClient> vClients = LoadClientsDataFromFile(ClientsFile);
	strClient Client;
	string AccountNumber = ReadClientNumber();

	while (!FindClientByAccountNumber(AccountNumber, Client,vClients)) {
		cout << "\nThe Client with (" << AccountNumber << ") Not Exist!\n";
		AccountNumber = ReadClientNumber();
	}
	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter withdraw amount? ";
	cin >> Amount;

	while (Amount > Client.AccountBalance) {
		cout << "\nAmount exeeds the balance, you can withdraw up to : ? " << Client.AccountBalance;
		cout << "\nPlease enter another amount? ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, -Amount, vClients);

}

void BackToTransactionMenu() {

	cout << "Press any key to back to transactions menu ..... ";
	system("pause>0");
	ShowTransactionsScreen();

}

void PerformTransactionsMenuOption(enTranscationsScreenMenu Option) {

	switch (Option) {

	case enTranscationsScreenMenu::enDeposit:

		system("cls");
		ShowDepositScreen();
		BackToTransactionMenu();

		break;

	case enTranscationsScreenMenu::enWithdraw:

		system("cls");
		ShowWithdrawScreen();
		BackToTransactionMenu();

		break;

	case enTranscationsScreenMenu::enTotalBalances:

		system("cls");
		ShowAllClientsBalancesScreen();
		BackToTransactionMenu();

		break;

	default:

		ShowMainMenu();

		return;
	}
}

void ShowTransactionsScreen() {

	system("cls");
	cout << "========================================\n";
	cout << "\t\tTransaction Menu Screen\n";
	cout << "========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "========================================\n";

	PerformTransactionsMenuOption(enTranscationsScreenMenu(ReadTransactionMenuOption()));
}

void PerformMainMenuOption(enMainScreenMenu Option) {

	switch (Option) {

	case enMainScreenMenu::enShowClientList:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainScreenMenu::enAddClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainScreenMenu::enDeleteClient:
		system("cls");
		ShowDeleteScreen();
		GoBackToMainMenu();
		break;
	case enMainScreenMenu::enUpdateClient:
		system("cls");
		ShowUpdateScreen();
		GoBackToMainMenu();
		break;
	case enMainScreenMenu::enFindClient:
		system("cls");
		ShowFindScreen();
		GoBackToMainMenu();
		break;
	case enMainScreenMenu::enTransactions:
		system("cls");
		ShowTransactionsScreen();
		GoBackToMainMenu();
		break;
	default:
		system("cls");
		EndScreen();
		return;
	}
}

void ShowMainMenu() {
	
	system("cls");
	cout << "========================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "========================================\n";

	PerformMainMenuOption(enMainScreenMenu(ReadMainMenuOption()));


}

int main()
{
	ShowMainMenu();

}

