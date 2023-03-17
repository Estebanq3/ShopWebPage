#include <iostream>
#include <cstring>
#include "web.h"
#include "connectDataBase.h"
#include <algorithm>

using namespace std;

int shoppingCartValidate(string producto,string usuario,string cantidad){
	int validTransaction = 0;
	if (view_article_existences(producto)>=stoi(cantidad)){
		string usuarioCarrito ="";
		usuarioCarrito = return_user_mail(usuario);
		add_to_shop_car(producto,stoi(cantidad),usuarioCarrito);
		validTransaction=1;
	}
	
	return validTransaction;
}


int main ()
{

string usuario = "";
string producto ="";
string cantidad ="";
string searchedText ="";

int sesion_finalizada = 0;

try{
char **qs = web_get_post_string();
char *product = web_get_from_query_string(qs, "productname");
char *quantity = web_get_from_query_string(qs, "cantidad");
char *cookies = web_get_cookies();
char *logout = web_get_from_query_string(qs, "cerrarSesion");
char *searchedProducts = web_get_from_query_string(qs, "search");

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




	if(!product ||!quantity){

	}else{
	producto=string(product);
	cantidad=string(quantity);
	}

	if(logout){
		sesion_finalizada = stoi(string(logout));
	}

	if(searchedProducts){
		searchedText=string(searchedProducts);
	}


}
}
catch(int error){

}

if(sesion_finalizada == 1){
	cout << "Set-Cookie:UserID = ;\r\n";
	cout << "Set-Cookie:SessionID = ;\r\n";
}

if(producto != "" && cantidad != "" && usuario != ""){

shoppingCartValidate(producto,usuario,cantidad);

}



cout << "Content-type:text/html\r\n\r\n";
cout << "<html>\n";
  
cout << "<link href='../main_page.css' rel='stylesheet' type='text/css'>\n";
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
cout << " <form action=\"main_page.cgi\" method=\"post\"> \n";
cout << " <input hidden type=\"text\" class=\"text\" name=\"cerrarSesion\" value=\"" << "1" << "\" style=\"display:none;\"> \n";
cout << " <button class=\"d\" ><i class=\"fa fa-user-times\" aria-hidden=\"true\"></i> Cerrar Sesion</button> \n";
cout << " </form> \n";


cout << "    </div> \n";
cout << "    </div> \n";

}
else{
cout << "  <form action=\"login_page.cgi\" method=\"post\">\n";
cout << "  <button class=\"b\" >Iniciar sesion</button>\n";
cout << " </form> \n";

}


cout << "    </div><br>\n";


cout << " <div class=\"search-container\" >\n";
cout << "    <form action=\"main_page.cgi\" method=\"post\">\n";
cout << "      <input type=\"text\" name=\"search\">\n";
cout << "         <button>Buscar</button>\n";
cout << "    </form>\n";
cout << "  </div><br><br><br>\n";

if(searchedText != ""){
cout << "<div class=\"results\" style=\"background-color: rgb(248,248,255,0.8)\">\n";
cout << "	  <span>Resultados para: \"" << searchedText << "\" </span>\n";
cout << "  </div>\n";
}



//Cerrar sesion, avanzar a pagina de login
if(sesion_finalizada == 1){
cout << "<div class=\"loader\"></div>\n";
cout << "<form hidden id=\"final_de_sesion\" action=\"login_page.cgi\">\n";
cout << "<script> document.getElementById(\"final_de_sesion\").submit(); </script> \n";
cout << " </form> \n";

}
 

cout << "<div class=\"row\" >\n";


string articulos[row_number_returner("Articulo")][column_number_returner("Articulo","Sitio_Ventas")];

string *ptr_articulos_matrix = articulos[0];

create_matrix_splitted("Articulo",column_number_returner("Articulo","Sitio_Ventas"), row_number_returner("Articulo"), ptr_articulos_matrix);

for(int i=0; i < row_number_returner("Articulo") ; ++i){
	
	if(articulos[i][0].find(searchedText)!=std::string::npos){

	cout << "  <div class=\"column\">\n";
	cout << "    <div class=\"card\">\n";
	cout << "	<img src=\"https://icon-library.com/images/product-icon/product-icon-8.jpg\" alt=\"Denim Jeans\">\n";
	if(articulos[i][0].length() > 15){
		cout << "	  <h2 >"<< articulos[i][0].substr(0,14) <<"..</h1>\n";
	}
	else{
		cout << "	  <h2 >"<< articulos[i][0] <<"</h1>\n";		
	}

	cout << "	  <p class=\"price\"> ₡" << articulos[i][1] << "</p>\n";
	cout << "	  <p>Existencias:" << articulos[i][2] << " </p>\n";
    cout << "  <form action=\"main_page.cgi\" method=\"post\">\n";
	if(usuario != ""){
	cout << "  <input type=\"number\" name=\"cantidad\" min=\"1\" max=\"" << view_article_existences(articulos[i][0]) << "\" > \n";
	cout << " <input hidden type=\"text\" class=\"text\" name=\"productname\" value=\"" << articulos[i][0] << "\" style=\"display:none;\">\n"; 
    cout << " <input hidden type=\"text\" class=\"text\" name=\"username\" value=\"" << usuario << "\" style=\"display:none;\">\n";
	cout << "         <p><button>Agregar al carrito</button></p>\n";	
    cout << "  </form>\n";
	}
	else{

	}


	cout << "	</div>\n";
	cout << "  </div>\n";

	}

	
}

 
cout << "</div>  \n";

    return 0;
}
