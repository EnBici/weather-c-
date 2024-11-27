#include<iostream>
#include<fstream>
#include<curl/curl.h>
#include"json.hpp"
#include<filesystem>
#include<string>

using namespace std;
using namespace std::filesystem;
using json = nlohmann::json;

string url,lol,naim,lon,lat,descripcion,clima,apiid;
int temp,vient,viento;


//put your user name 
string user ="" ;


path dir = "/home";
string file = ".weather.conf";
path fulldir = dir/user/file;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void getlocate(){
    CURL * curl;
    CURLcode res;
    curl = curl_easy_init();

    if(curl){
        curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteCallback);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&lol);
        res = curl_easy_perform(curl);

        auto jsonData = json::parse(lol);
        if (!jsonData.empty() && jsonData.is_array() && !jsonData[0].is_null()) {
            naim = jsonData[0].contains("name") ? jsonData[0]["name"].get<string>():"???";
            lat = jsonData[0].contains("lat") ? to_string(jsonData[0]["lat"].get<double>()) : "???";
            lon = jsonData[0].contains("lon") ? to_string(jsonData[0]["lon"].get<double>()) : "???";
    }
    

}
}

void getdatfile(){
    ifstream achis(fulldir);

    if(achis.is_open()){
        getline(achis,naim);
        getline(achis,lat);
        getline(achis,lon);
        getline(achis,apiid);
    }
}

void getusr(){
    cin>>user;
    ofstream achis(fulldir);

    if(achis.is_open()){
        achis<<user;
    }else{cout<<"?";}
    
}
void getdat(){
    string ciudad;
    string pais;
    cout<<"Ingrese la ciudad remplazando los espacios por %"<<endl;
    cin>>ciudad;
    cout<<"Ingrese su pais;D"<<endl;
    cin>>pais;
    cout<<"Ingrese su api id de openweathermap"<<endl;
    cin>>apiid;
    url = "http://api.openweathermap.org/geo/1.0/direct?q="+ciudad+","+pais+"&limit=1&appid="+apiid;
    system("clear");
    }
void guardarshit(){
    ofstream achis(fulldir);

    if(!exists(dir)){
        create_directory(dir);
    }
   
    if(achis.is_open()){
        achis<<naim<<endl<<lat<<endl<<lon<<endl<<apiid;
    }else{cout<<"hubo un error guardando la informacion";}

}
void climadata(){
    CURL * curl;
    CURLcode res;
    curl = curl_easy_init();
    string url2 = "https://api.openweathermap.org/data/2.5/weather?lat="+lat+"&lon="+lon+"&appid="+apiid;


    if(curl){
        
        curl_easy_setopt(curl,CURLOPT_URL,url2.c_str());
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteCallback);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&clima);
        res = curl_easy_perform(curl); 
        

        auto jsonData = json::parse(clima);
        if(jsonData["main"].contains("temp")){
            temp = jsonData["main"]["temp"].get<double>() - 273.15;  
        }
        if(jsonData.contains("weather") && !jsonData["weather"].empty()){
            descripcion = jsonData["weather"][0]["description"].get<string>();
        }
        if(jsonData.contains("name")){
            naim = jsonData["name"];
        }
        if(jsonData["wind"].contains("speed")){
            vient = jsonData["wind"]["speed"];
            viento = vient * 3,6;
        }
   

    cout<<"--------------"<<endl;
    cout<<naim<<endl;
    cout<<descripcion<<endl;
    cout<<"Temperatura: "<<temp<<"°C"<<endl;
    cout<<"viento: "<<viento<<"km/h"<<endl;;
    cout<<"--------------"<<endl;
    
        
}}


int main(){
    
  if(!exists(fulldir)){
        getdat();
        getlocate();
        guardarshit();
    }else{
        getdatfile();
        climadata();
        
    }  

}