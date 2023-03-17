#include <iostream>
#include <cstring>
#include "web.h"
#include "connectDataBase.h"

using namespace std;

int main ()
{
string usuario = "";
string contrasena = "";


try{
char **qs = web_get_post_string();
char *user = web_get_from_query_string(qs, "username");
char *password = web_get_from_query_string(qs, "password");	

	
if(!user || !password){
 throw 404;
}
else{
usuario = string(user);
contrasena = string(password);
}

}
catch(int error){

}



if(verify_account_existence(usuario,contrasena)){

cout << "Set-Cookie:UserID = " << usuario << "; \r\n";
cout << "Set-Cookie:SessionID = " << "session1" << ";\r\n";
//cout << "Set-Cookie:Expires = Mon, 23 May 2022 19:55:00 UTC;\r\n";

}

cout << "Content-type:text/html\r\n\r\n";
cout << "<html>\n";
  
cout << "<link href='../login_page.css' rel='stylesheet' type='text/css'>\n";
cout << "<div class=\"login\" style=\"background-color: rgb(248,248,255,0.8)\">\n";
cout << "<h2 class=\"active\"> Sitio de ventas </h2><br><br>\n";

char *cookies = web_get_cookies();
string cookies_string = "";
string cookies_splitted[2];


if(verify_account_existence(usuario,contrasena)){

cout << "<div class=\"loader\"></div>\n";
cout << "<form hidden id=\"loginCorrecto\" action=\"main_page.cgi\"  method=\"post\">\n";
cout << "<input hidden type=\"text\" class=\"text\" name=\"username\" value=\"" << usuario <<" \">\n";

cout << "<script> document.getElementById(\"loginCorrecto\").submit(); </script> \n";

}
else{
	if(usuario != ""){
		cout << "<div class=\"alert\">\n";
		cout << " Usuario o contraseña incorrecta.</div>\n";
		
	}

cout << "<form action=\"login_page.cgi\"  method=\"post\">\n"; 
}


cout << "<input type=\"text\" class=\"text\" name=\"username\">\n";
cout << "<span>Usuario</span>\n";
cout << "<br><br><br>\n";
cout << "<input type=\"password\" class=\"text\" name=\"password\">\n";
cout << "<span>Contraseña</span>\n";
cout << "<br>\n";
cout << "<a href=\"main_page.cgi\">Continuar como visitante</a>\n";
cout << "<button class=\"signin\">Ingresar</button>\n";
cout << "</form>\n";

cout << "<form action=\"register_page.cgi\" method=\"post\">\n";
cout << "<button class=\"register\" >Registrarse</button>\n";
cout << "</form>\n";
cout << "</div>\n";
 

    return 0;
}
