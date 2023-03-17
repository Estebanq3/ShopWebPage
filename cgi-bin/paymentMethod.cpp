#include <iostream>
#include <cstring>
#include "web.h"
#include "connectDataBase.h"
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
string correo = "";
string contra = "";
bool compra = "";
bool validateTransaction = false;
try{
char **qs = web_get_post_string();
char *shop = web_get_from_query_string(qs, "compra");
char *cookies = web_get_cookies();


if(!cookies){
 throw 404;
}
else{
compra= shop;
string cookies_string(cookies);
	string cookies_splitted[2];
	split_cookies(cookies_string,cookies_splitted);
	usuario = cookies_splitted[1];

	if(usuario != ""){
		if(usuario.at(0) == 's'){
			usuario = cookies_splitted[0];
		}
	}

}

}
 catch(int error){
}
if(usuario == ""){

cout << "<div class=\"loader\"></div>\n";
cout << "<form hidden id=\"cuentaInvalida\" action=\"main_page.cgi\">\n";
cout << " </form> \n";
cout << "<script> document.getElementById(\"cuentaInvalida\").submit(); </script> \n";


}
string carritoCompras[row_number_returner("CarritoCompras")][column_number_returner("CarritoCompras","Sitio_Ventas")];

string *ptr_carritoCompras_matrix = carritoCompras[0];

create_matrix_splitted("CarritoCompras",column_number_returner("CarritoCompras","Sitio_Ventas"), row_number_returner("CarritoCompras"), ptr_carritoCompras_matrix);
correo = return_user_mail(usuario);

cout << "Content-type:text/html\r\n\r\n";
cout << "<html>\n";
cout << "<link href='../register_page.css' rel='stylesheet' type='text/css'>\n";
cout << "<div class=\"login\" style=\"background-color: rgb(248,248,255,0.8);\" >\n";
if(!compra){
cout << "  <h2 class=\"active\"> Metodo de pago </h2>\n";
cout << "  <table class=\"cart\">	\n";
cout << "	<tr>\n";
cout << "    <th>Metodo de pago</th>\n";
cout << "   <th>Accion </th>\n";
cout << "	</tr>	\n";
cout << "       <tr>\n";
cout <<  "<td>" 
     << return_credit_card_by_mail(ltrim(rtrim(return_user_mail(usuario))))
     << " </td>";
cout <<  "<td>"  
        << "  <form action=\"paymentMethod_page.cgi\" method=\"post\" > " 
        << "<input hidden type=\"text\" class=\"text\" name=\"compra\" value=\"" << false << "\" style=\"display:none;\">" 
        << " 	<button class=\"cancel\" >Finalizar Compra</button>"
        << "  </form>"
        << " </td>"

<< "       </tr>   \n";   
cout << "</table>\n";
}else{
	cout << "  <h2 class=\"active\"> Factura </h2>\n";
for(int a = 0; a < row_number_returner("CarritoCompras"); a++)
{

	if(correo == rtrim(ltrim(carritoCompras[a][3]))){
        cout << "<span> "<< carritoCompras[a][1] <<" </span>";
        cout << "<span> "<< carritoCompras[a][2] <<" </span>";
        cout << "<span> "<< carritoCompras[a][3] <<" </span>";
        if(shoppingCartValidate(rtrim(ltrim(carritoCompras[a][2])),usuario,rtrim(ltrim(carritoCompras[a][1])))){
        cout << "<span> "<< "hola" <<" </span>";
        validateTransaction=true;
        cout << "<span> "<< validateTransaction <<" </span>";    
        }
        else{
        cout << "<span> "<< "hola2" <<" </span>";
        validateTransaction=false;
        cout << "<span> "<< validateTransaction <<" </span>";   
        }
		
    
    }
}
if(validateTransaction){
			for(int a = 0; a < row_number_returner("CarritoCompras"); a++)
			{
			if(correo == rtrim(ltrim(carritoCompras[a][3]))){
			update_delete_product(stoi(rtrim(ltrim(carritoCompras[a][1]))),rtrim(ltrim(carritoCompras[a][2])));
			cout << "<span> "<< "Proceso completado" <<" </span>";
			}
			}
		}else{
			cout << "<span> "<< "error" <<" </span>";
		}

}
cout << "  <form action=\"main_page.cgi\" method=\"post\" > \n";
cout << " 	<button class=\"cancel\" >Cancelar</button>\n";
cout << "  </form>\n";



cout << "</div>\n";




return 0;
}
