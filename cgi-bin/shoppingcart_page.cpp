#include <iostream>
#include <cstring>
#include "web.h"
#include "connectDataBase.h"
#include <math.h>
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


string getCorreo(string usuario){
 return return_user_mail(usuario);
}

int main ()
{
string usuario = "";
string correo = "";
string nombreCompleto = "";
string producto ="";
string cantidad ="";
string eliminar ="";

try{
char **qs = web_get_post_string();
char *user = web_get_from_query_string(qs, "username");
char *eliminateP = web_get_from_query_string(qs, "elimination");
char *product = web_get_from_query_string(qs, "productname");
char *quantity = web_get_from_query_string(qs, "cantidad");	
char *cookies = web_get_cookies();

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


if(!eliminateP){

}else{

eliminar =string(eliminateP);  
correo = return_user_mail(usuario);
delete_from_shop_car(eliminar,correo);
}

}

}
catch(int error){

}


if(producto != "" && cantidad != ""){

shoppingCartValidate(producto,usuario,cantidad);

}


string carritoCompras[row_number_returner("CarritoCompras")][column_number_returner("CarritoCompras","Sitio_Ventas")];

string *ptr_carritoCompras_matrix = carritoCompras[0];

create_matrix_splitted("CarritoCompras",column_number_returner("CarritoCompras","Sitio_Ventas"), row_number_returner("CarritoCompras"), ptr_carritoCompras_matrix);
correo = return_user_mail(usuario);




cout << "Content-type:text/html\r\n\r\n";
cout << "<html>\n";
cout << "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\"> \n";
cout << "<link href='../register_page.css' rel='stylesheet' type='text/css'>\n";
cout << "<div class=\"login\" style=\"background-color: rgb(248,248,255,0.8);\" >\n";

cout << "  <h2 class=\"active\">  Carrito de Compras </h2>\n";

if(usuario == ""){

cout << "<div class=\"loader\"></div>\n";
cout << "<form hidden id=\"cuentaInvalida\" action=\"main_page.cgi\">\n";
cout << " </form> \n";
cout << "<script> document.getElementById(\"cuentaInvalida\").submit(); </script> \n";

}

cout << "   	<table class=\"cart\">	\n";

cout << "	<tr>\n";
cout << "    <th>Producto</th>\n";
cout << "   <th>Cantidad</th>\n";
cout << "   <th>Precio</th>\n";
cout << "    <th>Accion</th>\n";
cout << "    <th>Accion</th>\n";
cout << "	</tr>	\n";


for(int a = 0; a < row_number_returner("CarritoCompras"); a++)
    {

	if(correo == rtrim(ltrim(carritoCompras[a][3]))){
        cout << "       <tr>\n";
	cout << "   <td>" << carritoCompras[a][2] << "</td>\n";
        cout << "   <td>" << carritoCompras[a][1] << "</td>\n";
        cout << "   <td>" << "₡"<<  stoi(carritoCompras[a][1]) * return_product_price(rtrim(ltrim(carritoCompras[a][2]))) << " </td>\n";
        
        
        cout <<"<td>" 
        << "  <form action=\"shoppingcart_page.cgi\" method=\"post\" > ";
        cout << "  <input type=\"number\" name=\"cantidad\" min=\"1\" max=\"" << view_article_existences(rtrim(ltrim(carritoCompras[a][2]))) << "\" > \n"; 
        
        cout << "  <input hidden type=\"text\" class=\"text\" name=\"productname\" value=\"" << rtrim(ltrim(carritoCompras[a][2])) << "\" style=\"display:none;\">"
        << "  <button><i class=\"fa fa-pencil fa-3x\" aria-hidden=\"true\"></i> </button>"
        << "  </form>"
        << " </td>";
        


        cout <<  "<td>"  
        << "  <form action=\"shoppingcart_page.cgi\" method=\"post\" > " 
        << "<input hidden type=\"text\" class=\"text\" name=\"elimination\" value=\"" << rtrim(ltrim(carritoCompras[a][2]))<< "\" style=\"display:none;\">" 
        << " 	<button ><i class=\"fa fa-times-circle-o fa-3x\" aria-hidden=\"false\"  >  </i></button>"
        << "  </form>"
        << " </td>"
        
        	    
        << "       </tr>   \n";   
	}
    }

cout << "</table>\n";
cout << "	<br><br><br>	\n";

cout << "  <form action=\"paymentMethod_page.cgi\" method=\"post\" style=\"padding-top: 40px\">  \n";
bool validateTransaction = false;
for(int a = 0; a < row_number_returner("CarritoCompras"); a++)
{

	if(correo == rtrim(ltrim(carritoCompras[a][3]))){
        if(shoppingCartValidate(rtrim(ltrim(carritoCompras[a][2])),usuario,rtrim(ltrim(carritoCompras[a][1])))){
        validateTransaction=true;  
        }
        else{
        validateTransaction=false;  
        }
    
    }
}
if(validateTransaction){
 string factura ="";
}
cout << "  <button class=\"create\">Procesar Compra</button>\n";
cout << " </form>\n";

cout << "  <form action=\"main_page.cgi\" method=\"post\" > \n";
cout << " 	<button class=\"cancel\" >Cancelar</button>\n";
cout << "  </form>\n";


cout << "</div>\n";
    return 0;
}


