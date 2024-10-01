#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

//=====================================================================================
//structs needed
struct Exp{//expiration date struct
    int day;
    int month;
    int year;
};

//classes
//products
class Product {       // to describe the information of each product
private://variables
    string name;// name of the product
    double price;// price of the product
    int count;// amount of the product
    string outlet;// outlet of the product
    Exp Expdate;// Expiration date of the product

public://functions

    //constructor
    Product(string Name, double Price, int Count, string Outlet,struct Exp EXP)
    {
        name=Name;
        price=Price;
        count=Count;
        outlet=Outlet;
        Expdate = EXP;
    }
//=====================================================================================
    //setters
    void SetName(string Name)  {
        name=Name;
    }
    void SetPrice(double Price)  {
        price=Price;
    }
    void SetCount(int Count){
        count=Count;
    }
    void SetOutlet(string Outlet){
        outlet=Outlet;
    }
//=====================================================================================
    //getters
    string getName()  {
        return name;
    }

    double getPrice()  {
        return price;
    }

    int getCount()  {
        return count;
    }

    string getOutlet()  {
        return outlet;
    }
    struct Exp getExpdate () {
        return Expdate;
    }
//======================================================================================
    // dispensing function
    void dispense(){
        cout << "Dispensing from " << outlet << ": " << name << endl;
    }
    bool is_expired(){
        time_t now = time(0);
        tm *ltm = localtime(&now);   // local time at the moment

        int year =1900 + ltm->tm_year,month= 1 + ltm->tm_mon,day=ltm->tm_mday;   // current year, month, day
        if(Expdate.year < year){
            return true;
        }
        else if(Expdate.year == year && Expdate.month < month){
            return true;
        }
        else if(Expdate.year == year && Expdate.month == month && Expdate.day <day){
            return true;
        }
        return false;
    }
};


//=====================================================================================
// VendingMachine class to manage the vending machine operations
class VendingMachine{
private://variables
    vector<Product> products;     // the list of products in the vending machine
    double depositedAmount;       // the money deposited by the user

public:
    VendingMachine(){
        depositedAmount=0.0;
    }
//========================================================================================
    // Programming mode functions
    Product take_info(){
        string type,outlet;
        double price;
        int count;
        Exp temp_exp;
        cout<< "please, Enter the type: "<<endl; // name of added product
        cin.ignore();
        getline(cin,type);
        cout<< "please, Enter the price: "<<endl;
        cin >> price;                            // its price
        cout<< "please, Enter the count: "<<endl;
        cin >> count;                            // the number of products
        cout<< "please, Enter the outlet: "<<endl;
        cin >> outlet;                           // its outlet in machine
        cout<< "please, Enter the the date of expiry(dd mm yy):  "<<endl;
        cin >> temp_exp.day>> temp_exp.month>>temp_exp.year;       // expiration date
        return Product(type, price, count, outlet,temp_exp);
    }

    bool Passcheck(){ // to ensure that only the vendor could access the machine's data
        int x; // password
        cout<< "please enter the password the 4 digit password ";
        cin>> x;
        cout<<endl;
        if(x == 1234)
            return 1;
        for(int i =0;i<2;i++){   // additional 2 trials for the user
            cout<< "Wrong password!!"<<endl;
            cout<< "you have only "<< 2-i<< " tries left"<<endl;
            cout<< "enter the password again"<<endl;
            cin>>x;
            cout<<endl;
            if(x==1234)
                return 1;
        }
        cout<< "Sorry, you don't have access."<<endl;  // wrong password
        return 0;
    }
    void addProduct(const Product& product){ // to add the product to the list (in the vector)
        products.push_back(product);
    }
    void changeProduct(const Product& product, int index){ // to add the product to the list (in the vector)
        products[index] = product;
    }
//===================================================================================================
//===================================================================================================
    //Normal mode
    void displayProducts(){ // displaying all the products in the machine
        cout<< "Here is all the products we have: "<<endl;
        cout<<"======================================================================"<<endl;
        cout << "Available products:" << endl;
        int i =1;

        for (auto & product : products) {

            if(product.getCount()== 0){i++; continue;}
            // if the product is unavailable, don't display it

            cout << i<< "- Name: " << product.getName() << ", Price: " << product.getPrice() << ", Amount: "<< product.getCount() << endl;
            // print name, price, amount of product
            cout<< "========================================="<<endl;
            i++;
        }
        cout<<"======================================================================"<<endl;
    }
    void selectProduct(int index) {   // selecting product based on its index in the vector
        if (index >= 0 && index < products.size()) {      //ensure that the user's choice is within limits
            Product& selectedProduct = products[index];

            if(selectedProduct.is_expired()){
                cout<< "This Product is expired. Please, Choose something else."<<endl;
                return;
            }

            if (selectedProduct.getCount() > 0) {   //check if available
                cout << "You selected: " << selectedProduct.getName() << endl;
                collectMoney(selectedProduct); // calling the payment function
            }
            else {  // unavailable
                cout << "Product is out of stock." << endl;
            }
        }
        else {
            cout << "Invalid product selection." << endl;
        }
    }

    void collectMoney(Product& product) {  // deposit the price
        cout << "Please deposit $" << product.getPrice() << endl;
        double deposited;
        cin >> deposited; // user's money deposited

        if (deposited >= product.getPrice()) { // check if the payment is enough
            depositedAmount += deposited;  // total money deposited from the user
            dispenseProduct(product);  // dispensing the product
        }
        else {
            cout << "Insufficient amount. Please deposit the required amount." << endl;  // not enough
        }
    }
    void dispenseProduct(Product& product) {
        product.dispense();
        product.SetCount(product.getCount()-1);    // the count of products decreased by 1
        double change = depositedAmount - product.getPrice();

        if (change > 0) {    // check if there is a change
            cout << "Returning change: $" << change << endl;
        }

        depositedAmount = 0.0;
    }
//===============================================================================================
//===============================================================================================
    //interface
    int start(){
        int mode;   // programming of operating mode?
        cout << "Which mode do you want? " << endl;
        cout<<"please, choose a number between 1 and 2."<<endl;
        cout<< "1. Programming mode ------------ 2. normal mode "<<endl;
        cout<< "========================================"<<endl;
        cin >> mode;
        return mode;
    }
    int end(){
        // check if the user finished his services
        cout<< "Thanks for using our vending machine. "<<endl;
        cout<<"================================================"<<endl;
        cout<< "If you don't want any other services, enter -1."<<endl;
        cout<< "Enter the number: ";
        int x;
        cin>>x;
        return x;
    }
//========================================================================================
    //Maintenance mode
    void CheckExp(){  // check the validity of the product
        cout<< "Do you want to check Exp of the products?"<<endl;
        cout<< "1.Yes----------------2.No"<<endl;
        cout<< "Enter a number: ";
        int x;
        cin>>x;
        if(x==2) return;   // don't care about its expiry

        for (auto& product : products) {
            //  if expired, don't count it
            if(product.is_expired())product.SetCount(0);
        }
        cout<< "========================================================"<<endl;
        cout<< "The Products have been checked successfully!"<<endl;
    }
};

int main() {


    VendingMachine vendingMachine;
    //set up the machine
    vendingMachine.addProduct(Product("Soda", 1.5, 5, "Outlet 1",(Exp){1,1,2025}));
    vendingMachine.addProduct(Product("Water", 1.0, 10, "Outlet 2",(Exp){16,8,2015}));
    vendingMachine.addProduct(Product("Juice", 2.0, 3, "Outlet 3",(Exp){12,5,2030}));

    while(1) {  // while the user isn't ending the process
        if (vendingMachine.start() == 1){
            if(vendingMachine.Passcheck()) {    // check the password
                cout<< "Choose what you want to do"<<endl;
                cout<< "1. check expiration dates  =========="<<"   2. update products"<<endl;
                int x;
                cin>>x;
                if(x==2) {
                    cout<<"1. Adding a new product           2. modifying an existing product"<<endl;
                    int y;
                    cin>>y;
                    if(y==1)
                        vendingMachine.addProduct(vendingMachine.take_info());  //add a new product with the info set by the vendor
                    else if(y==2){
                        cout<<"Choose index"<<endl;
                        int indx;
                        cin>>indx;
                        vendingMachine.changeProduct(vendingMachine.take_info(),indx-1);
                    }
                }
                else {
                    vendingMachine.CheckExp();      // check the expiration date
                }
            }
        }
        else{
            vendingMachine.displayProducts();   // display all products for the user
            int index;                          // the user choose the product by index
            cout << "Enter your selection" << endl;
            cout<< "(please, choose the one you want by index): "<<endl;
            cin >> index;
            vendingMachine.selectProduct(index-1); // implementing the order
        }
        if(vendingMachine.end()==-1)break;    // end of process

    }
    return 0;
}