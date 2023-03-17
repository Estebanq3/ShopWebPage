#include <iostream>
#include <cstring>
#include <string>
#include "web.h"
#include "connectDataBase.h"

using namespace std;


int validateAccountCreationData(string username, string password, string email, string firstName, string numero, string firsLastName, string secondLastName, string address){
	int validData = 0;
	
	if(username.length()>= 0 && username.length()< 21 && password.length() >= 8 && email.length() >= 0 && email.length() < 41 && firstName.length() >= 0 && firstName.length() < 31 && firsLastName.length() >= 0 && firsLastName.length() < 31 && secondLastName.length() >= 0 && secondLastName.length() < 31){

		validData = create_information_user(email,firsLastName,secondLastName,firstName,stoi(numero), address,username,password);
		
	}
	return validData;
}


int main ()
{
string usuario = "";
string contrasena = "";
string correo = "";
string numeroTelefono = "";
string nombre = "";
string primerApellido = "";
string segundoApellido = "";
string direccion = "";

try{
char **qs = web_get_post_string();
char *user = web_get_from_query_string(qs, "username");
char *password = web_get_from_query_string(qs, "password");
char *email = web_get_from_query_string(qs, "correo");
char *numero = web_get_from_query_string(qs, "numeroTel");
char *first_name = web_get_from_query_string(qs, "nombre");
char *first_last_name = web_get_from_query_string(qs, "primerApellido");
char *second_last_name = web_get_from_query_string(qs, "segundoApellido");
char *address = web_get_from_query_string(qs, "direccion");

if(!user || !password || !email || !first_name || !first_last_name || !second_last_name || !numero || !address){
 throw 404;
}
else{
usuario = string(user);
contrasena = string(password);
correo = string(email);
numeroTelefono = string(numero);
nombre = string(first_name);
primerApellido = string(first_last_name);
segundoApellido = string(second_last_name);
direccion = string(address);

}


}
catch(int error){

}

cout << "Content-type:text/html\r\n\r\n";
cout << "<html>\n";
 
 
cout << " <link href='../register_page.css' rel='stylesheet' type='text/css'>\n";

cout << " <div class=\"login\" style=\"background-color: rgb(248,248,255,0.8); height:1100px;\" >\n";
cout << "  <h2 class=\"active\"> Registro de Cuenta </h2>\n";


if(validateAccountCreationData(usuario, contrasena, correo, nombre, numeroTelefono, primerApellido, segundoApellido, direccion)){

cout << "<div class=\"loader\"></div>\n";
cout << "<form hidden id=\"registroCorrecto\" action=\"login_page.cgi\"  method=\"post\">\n";
cout << "<script> document.getElementById(\"registroCorrecto\").submit(); </script> \n";

}
else{
	if(usuario != ""){
		cout << "<div class=\"alert\">\n";
		cout << "	Datos incompletos, por favor revise los datos ingresados.</div>\n";
		
	}
	
cout << "  <form action=\"register_page.cgi\" method=\"post\" style=\"padding-top: 40px\">\n";
cout << "    <input type=\"text\" class=\"text\" name=\"username\" maxlength=\"20\" required>\n";
cout << "    <span>Usuario</span><br><br><br>\n";
cout << "   <input type=\"password\" class=\"text\" name=\"password\" maxlength=\"15\" required >\n";
cout << "    <span>Contraseña</span><br><br><br>\n";
cout << "    <input type=\"email\" class=\"text\" name=\"correo\" maxlength=\"40\" required>\n";
cout << "    <span>Correo</span><br><br><br>\n";
cout << "    <input type=\"text\" class=\"text\" name=\"numeroTel\" maxlength=\"8\" required>\n";
cout << "    <span>Telefono</span><br><br><br>\n";
cout << "       <input type=\"text\" class=\"text\" name=\"nombre\" maxlength=\"30\" required>\n";
cout << "    <span>Nombre</span><br><br><br>\n";
cout << "    <input type=\"text\" class=\"text\" name=\"primerApellido\" maxlength=\"30\" required>\n";
cout << "    <span>Primer apellido</span><br><br><br>\n";
cout << "       <input type=\"text\" class=\"text\" name=\"segundoApellido\" maxlength=\"30\" required>\n";
cout << "    <span>Segundo apellido</span><br><br><br>\n";
cout << "       <input type=\"text\" class=\"text\" name=\"direccion\" maxlength=\"250\" required>\n";
cout << "    <span>Direccion</span><br><br><br>\n";
cout << "    <button class=\"create\">Crear cuenta</button>\n";
cout << "  </form>\n";
cout << "  <form action=\"login_page.cgi\">\n";
cout << "       <button class=\"cancel\" >Cancelar</button>\n";
cout << "  </form>\n";
cout << " </div>\n";

}
 

    return 0;
}

