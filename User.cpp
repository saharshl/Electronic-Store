/*
Project Title: Electronics Store 
Version      : 1.0
Developed By : Suyash Lohia
School       : Delhi Public School R.K. Puram
*/

#include<fstream.h>
#include<conio.h>
#include<string.h>
#include<stdio.h>
#include<iomanip.h>

class Product
{
 int Pno,Bno,Qty;
 char Pname[40],Brand[40];
 float Price;
 public:
 void Input();
 void InputPno(){cin>>Pno;}
 void InputBno(){cin>>Bno;}
 void InputBrand(){gets(Brand);}
 void InputPrice(){cin>>Price;}
 void InputQty(){cin>>Qty;}
 void Output();
 void AddQty(int);
 void ReduceQty(int);
 int RPno(){return Pno;}
 int RBno(){return Bno;}
 char* RPname(){return Pname;}
 char* RBrand(){return Brand;}
 float RPrice(){return Price;}
 int RQty(){return Qty;}
};

void Admin();
void Customer();
void Add();
void AddQty1();
void AddQty2(int,int);
void Delete();
void Edit();
void Search();
void DisplayAll();
void Billing(int,int);
float CalculateTotal(int,float);

void main()
{
 textbackground(WHITE);
 textcolor(BLACK);
 clrscr();
 int ch;
 char PWch;
 char PW[20];
 cout<<setw(50)<<"ELECTRONICS STORE \n \n \n";
 do
 {
  cout<<"\n 1.Admin \n";
  cout<<"2.Customer \n";
  cout<<"3.Quit \n\n";
  cout<<"Enter your choice: ";
  cin>>ch;
  switch(ch)
  {
   case 1:
   {
    int i=0;
    cout<<"Enter password: ";
    do
    {
     PWch=getch();
     cout<<'*';
     PW[i++]=PWch;
    }while(PWch!=13);
    PW[i-1]='\0';
    if(strcmp(PW,"password")==0)
      Admin();
    else
      cout<<"Incorrect password - *ACCESS DENIED*"<<endl;
   }
   break;
   case 2:
    Customer();
   break;
   case 3:
    cout<<"Quitting...";
  }
 }while(ch!=3);
 getch();
}

void Admin()
{
 textcolor(BLACK);
 textbackground(WHITE);
 int ch;
 do
 {
  cout<<"\n 1.Add product \n";
  cout<<"2.Add quantity \n";
  cout<<"3.Delete \n";
  cout<<"4.Edit \n";
  cout<<"5.Search \n";
  cout<<"6.Display all \n";
  cout<<"7.Back \n";
  cout<<"\n Enter your choice: ";
  cin>>ch;
  switch(ch)
  {
   case 1:Add();
   break;
   case 2:AddQty1();
   break;
   case 3:Delete();
   break;
   case 4:Edit();
   break;
   case 5:Search();
   break;
   case 6:DisplayAll();
  }
 }while(ch!=7);
}

void Customer()
{
 textcolor(BLACK);
 textbackground(WHITE);
 fstream fil;
 fil.open("Product.dat",ios::binary|ios::in);
 Product P;
 int Pnum,Bnum;
 while(fil.read((char*)&P,sizeof(P)))
   P.Output();
 cout<<"\n Enter product number: ";
 cin>>Pnum;
 cout<<"Enter brand number: ";
 cin>>Bnum;
 fil.close();
 Billing(Pnum,Bnum);
}

void Billing(int Pnum,int Bnum)
{
 textcolor(BLACK);
 textbackground(WHITE);
 fstream fil;
 fil.open("Product.dat",ios::binary|ios::in|ios::app);
 Product P;
 int Num,Total,Found=0;
 do
 {
  while(fil.read((char*)&P,sizeof(P)));
  if(P.RPno()==Pnum && P.RBno()==Bnum)
  {
   cout<<"\n Enter quantity: ";
   cin>>Num;
   if(P.RQty()<Num)
     cout<<endl<<Num<<" pieces unavailable";
   else
   {
    fil.seekg(fil.tellg()-sizeof(P));
    P.ReduceQty(Num);
    fil.write((char*)&P,sizeof(P));
    Total=CalculateTotal(Num,P.RPrice());
    Found++;
   }
  }
 }while(!Found);
 cout<<"\n Total price= Rs. "<<Total;
 fil.close();
}

void Add()
{
 fstream fil;
 fil.open("Product.dat",ios::binary|ios::app);
 Product P;
 P.Input();
 fil.write((char*)&P,sizeof(P));
 fil.close();
}

void AddQty1()
{
 fstream fil;
 fil.open("Product.dat",ios::binary|ios::in|ios::out);
 Product P;
 int Pnum,Bnum,Num;
 while(fil.read((char*)&P,sizeof(P)))
   P.Output();
 cout<<"\n Enter product number: ";
 cin>>Pnum;
 cout<<"Enter brand number: ";
 cin>>Bnum;
 fil.close();
 AddQty2(Pnum,Bnum);
}

void AddQty2(int Pnum,int Bnum)
{
 fstream fil;
 fil.open("Product.dat",ios::binary|ios::in|ios::out);
 Product P;
 int Num;
 while(fil.read((char*)&P,sizeof(P)))
 {
  if(P.RPno()==Pnum && P.RBno()==Bnum)
  {
   cout<<"\n Add how many? ";
   cin>>Num;
   fil.seekg(fil.tellg()-sizeof(P));
   P.AddQty(Num);
   fil.write((char*)&P,sizeof(P));
  }
 }
 fil.close();
}

void Delete()
{
 fstream f1,f2;
 f1.open("Product.dat",ios::binary|ios::in);
 f2.open("Temp.dat",ios::binary|ios::out);
 Product P;
 int Num,Del=0;
 cout<<"Enter brand number to be deleted: ";
 cin>>Num;
 while(f1.read((char*)&P,sizeof(P)))
 {
  if(P.RPno()!=Num)
    f2.write((char*)&P,sizeof(P));
  else
    Del++;
 }
 f1.close();
 f2.close();
 if(Del)
 {
  remove("Product.dat");
  rename("Temp.dat","Product.dat");
 }
 else
   remove("Temp.dat");
}

void Edit()
{
 fstream fil;
 fil.open("Product.dat",ios::binary|ios::in|ios::out);
 Product P;
 int Num,Found=0;
 cout<<"\n Enter brand number to be changed: ";
 cin>>Num;
 while(fil.read((char*)&P,sizeof(P)) && !Found)
 {
  if(P.RBno()==Num)
  {
   P.InputBno();
   fil.seekg(fil.tellg()-sizeof(P));
   fil.write((char*)&P,sizeof(P));
   Found++;
  }
 }
 if(Found)
   cout<<"Record edited";
 else
   cout<<"Record not found";
 fil.close();
}

void Search()
{
 fstream fil;
 fil.open("Product.dat",ios::binary|ios::in);
 Product P;
 int ch;
 cout<<"\n Search products by: 1.Product number 2.Product name 3.Brand number 4.Brand name 5.Price \n";
 cout<<"Enter your choice: ";
 cin>>ch;
 switch(ch)
 {
  case 1:
  {
   int Num;
   cout<<"\n Enter product number to be searched: ";
   cin>>Num;
   while(fil.read((char*)&P,sizeof(P)))
   {
    if(P.RPno()==Num)
      P.Output();
   }
  }
  break;
  case 2:
  {
   char Name[40];
   cout<<"\n Enter product name to be searched: ";
   gets(Name);
   while(fil.read((char*)&P,sizeof(P)))
   {
    if(strcmpi(P.RPname(),Name)==0)
      P.Output();
   }
  }
  break;
  case 3:
  {
   int Num;
   cout<<"\n Enter brand number to be searched: ";
   cin>>Num;
   while(fil.read((char*)&P,sizeof(P)))
   {
    if(P.RPno()==Num)
      P.Output();
   }
  }
  break;
  case 4:
  {
   char Name[40];
   cout<<"\n Enter brand name to be searched: ";
   gets(Name);
   while(fil.read((char*)&P,sizeof(P)))
   {
    if(strcmpi(P.RBrand(),Name)==0)
      P.Output();
   }
  }
  break;
  case 5:
  {
   int Num;
   cout<<"\n Enter price to be searched: ";
   cin>>Num;
   while(fil.read((char*)&P,sizeof(P)))
   {
    if(P.RPrice()==Num)
      P.Output();
   }
  }
 }
 fil.close();
}

void DisplayAll()
{
 fstream fil;
 fil.open("Product.dat",ios::binary|ios::in);
 Product P;
 while(fil.read((char*)&P,sizeof(P)))
   P.Output();
 fil.close();
}

float CalculateTotal(int Num,float Price)
{
 float Total=Num*Price;
 return Total;
}

void Product::Input()
{
 cout<<"Product number: ";
 cin>>Pno;
 cout<<"Product name: ";
 gets(Pname);
 cout<<"Brand number: ";
 cin>>Bno;
 cout<<"Brand: ";
 gets(Brand);
 cout<<"Price: ";
 cin>>Price;
 cout<<"Quantity: ";
 cin>>Qty;
}

void Product::Output()
{
 cout<<endl<<Pno<<endl;
 puts(Pname);
 cout<<Bno<<endl;
 puts(Brand);
 cout<<"Rs. "<<Price<<endl;
 cout<<Qty<<endl;
}

void Product::AddQty(int Num)
{
 Qty=Qty+Num;
}

void Product::ReduceQty(int Num)
{
 Qty=Qty-Num;
}

