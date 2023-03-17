#include <iostream>
#include <cstring>
#include "web.h"
#include "connectionManager2.h"

using namespace std;


int validateProductData(string productName, string cost, string existenceAmount){
	int validData = 0;
	
	if((productName.length() > 0 && productName.length() < 51 && cost.length() > 0 && existenceAmount.length() > 0) && view_article_existences(productName) < 1){
 
		insert_product(productName, stod(cost), stoi(existenceAmount));
		validData = 1;
		
		
	}
	return validData;
}

int main ()
{

string usuario = "";
string nombreProducto = "";
string costo = "";
string existencias = "";
int sesion_finalizada = 0;


// Excepciones

try{
char **qs = web_get_post_string();
char *name = web_get_from_query_string(qs, "productName");
char *cost = web_get_from_query_string(qs, "productCost");
char *amount = web_get_from_query_string(qs, "existenceAmount");
char *cookies = web_get_cookies();
char *logout = web_get_from_query_string(qs, "cerrarSesion");

if(logout){
	sesion_finalizada = stoi(string(logout));
}	

if(!cookies){
 throw 404;
}
else{

	string cookies_string(cookies);
	string cookies_splitted[2];
	split_cookies(cookies_string,cookies_splitted);
	usuario = cookies_splitted[1];

	if(usuario != ""){
		if(usuario.at(0) == 's'){
			usuario = cookies_splitted[0];
		}
	}

	if(!name || !cost || !amount){
	 throw 404;
	}
	else{
		nombreProducto = string(name);
		costo = string(cost);
		existencias = string(amount);
		
	}
}
}
catch(int error){
}


if(sesion_finalizada == 1){
	cout << "Set-Cookie:UserID = ;\r\n";
	cout << "Set-Cookie:SessionID = ;\r\n";
}

//Codigo html

cout << "Content-type:text/html\r\n\r\n";
cout << "<html>\n";

cout << "<link href='../product_sale_page.css' rel='stylesheet' type='text/css'>\n";
cout << "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\"> \n";

cout << "<div class=\"topnav\">\n";
cout << "  <form action=\"main_page.cgi\">\n";
cout << "  <button class=\"b\" >Inicio</button>\n";
cout << "  </form>\n";

cout << "  <form action=\"product_page.cgi\">\n";
cout << "  <button class=\"b\" >Retroalimentacion</button>\n";
cout << "  </form>\n";

cout << "  <form action=\"product_page.cgi\">\n";
cout << "  <button class=\"b\" >Acerca de nosotros</button>\n";
cout << "  </form>\n";


if(usuario != ""){
cout << "  <form action=\"product_page.cgi\">\n";
cout << "  <button class=\"b\" >Venta de articulo</button>\n";
cout << "  </form>\n";

cout << "  <div class=\"dropdown\">\n";
cout << "  <button class=\"dropbtn\">"<< usuario << "<i class=\"fa fa-caret-down\"></i></button> \n";
cout << "   <div class=\"dropdown-content\">  \n";

cout << " <form action=\"product_page.cgi\">\n";
cout << " <button class=\"d\" ><i class=\"fa fa-user-circle\"></i> Mi Cuenta</button>\n";
cout << " </form>\n";


cout << " <form action=\"shoppingcart_page.cgi\" method=\"post\"> \n";
cout << " <input hidden type=\"text\" class=\"text\" name=\"username\" value=\"" << usuario << "\" style=\"display:none;\"> \n";
cout << " <button class=\"d\" ><i class=\"fa fa-shopping-cart\"></i> Carrito de compras</button> \n";
cout << " </form> \n";

//Cerrar sesion
cout << " <form action=\"product_page.cgi\" method=\"post\"> \n";
cout << " <input hidden type=\"text\" class=\"text\" name=\"cerrarSesion\" value=\"" << "1" << "\" style=\"display:none;\"> \n";
cout << " <button class=\"d\" ><i class=\"fa fa-user-times\" aria-hidden=\"true\"></i> Cerrar Sesion</button> \n";
cout << " </form> \n";


cout << "    </div> \n";
cout << "    </div> \n";

}
else{
cout << "<div class=\"loader\"></div>\n";
cout << "<form hidden id=\"cuentaInvalida\" action=\"main_page.cgi\">\n";
cout << " </form> \n";
cout << "<script> document.getElementById(\"cuentaInvalida\").submit(); </script> \n";

}
cout << "    </div><br><br><br> \n";

//Cerrar sesion, avanzar a pagina de login
if(sesion_finalizada == 1){
cout << "<div class=\"loader\"></div>\n";
cout << "<form id=\"final_de_sesion\" action=\"login_page.cgi\">\n";
cout << "<script> document.getElementById(\"final_de_sesion\").submit(); </script> \n";
cout << " </form> \n";

}


if(validateProductData(nombreProducto,costo,existencias)){

cout << "<div class=\"loader\"></div>\n";
cout << "<form hidden id=\"registroCorrecto\" action=\"main_page.cgi\"  method=\"post\">\n";
cout << " <input hidden type=\"text\" class=\"text\" name=\"username\" value=\"" << usuario << "\" style=\"display:none;\">\n";
cout << " </form> \n";
cout << "<script> document.getElementById(\"registroCorrecto\").submit(); </script> \n";

}
else{
    if(nombreProducto != "" || costo != "" || existencias !=""){
        cout << "<div class=\"alert\">\n";
        cout << "       Datos incorrectos, por favor revise los datos ingresados.</div>\n";
    }	
		
	cout << " <div class=\"login\" style=\"background-color: rgb(248,248,255,0.8);\" >\n";
	cout << "  <h2 class=\"title\"> Poner Articulo En Venta </h2>\n";
	
	cout << "  <form method=\"post\" style=\"padding-top: 40px\" action=\"product_page.cgi\">\n";
	
	cout << "   <input type=\"text\" class=\"text\" name=\"productName\" maxlength=\"51\" required>\n";
	cout << "    <span>Nombre del articulo</span><br><br><br>\n";
	
	cout << "    <input type=\"text\" class=\"text\" name=\"productCost\" maxlength=\"10\" required pattern=\"^[0-9]+(\.[0-9]{1,2})?$\" title=\"Asegurese de digitar solo montos validos sin comas.\">\n";
	cout << "    <span>Precio</span><br><br><br>\n";
	
	cout << "    <input type=\"text\" class=\"text\" name=\"existenceAmount\" maxlength=\"9\" required pattern=\"[0-9]+\" title=\"Asegurese de digitar solo numeros.\">\n";
	cout << "    <span>Numero de existencias</span><br><br><br>\n";
	
	cout << " <input hidden type=\"text\" class=\"text\" name=\"username\" value=\"" << usuario << "\" style=\"display:none;\">\n";
	cout << "    <button class=\"create\">Confirmar</button>\n";
	cout << "  </form>\n";
	
	
	cout << "  <form action=\"main_page.cgi\" method=\"post\">\n";
    cout << " <input hidden type=\"text\" class=\"text\" name=\"username\" value=\"" << usuario << "\" style=\"display:none;\">\n";
	cout << "  	<button class=\"cancel\" >Cancelar</button>\n";
	cout << "  </form>\n";
	cout << " </div>\n";
	
}


    return 0;
}


