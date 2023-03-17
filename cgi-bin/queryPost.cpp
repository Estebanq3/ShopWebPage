#include "web.h"
#include <iostream>
#include </usr/include/mysql/mysql.h>
#include <cstring>

using namespace std;


struct connection_details
{
    const char *server, *user, *password, *database;
};

MYSQL* mysql_connection_setup(struct connection_details mysql_details){
    MYSQL *connection = mysql_init(NULL); // mysql instance
    
    //connect database
    if(!mysql_real_connect(connection, mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0)){
        cout << "Connection Error: " << mysql_error(connection) << endl;
        exit(1); 
    }

    return connection;
}

// mysql_res = mysql result
MYSQL_RES* mysql_perform_query(MYSQL *connection, const char *sql_query){
    //send query to db
    if(mysql_query(connection, sql_query)){
        cout << "MySQL Query Error: " << mysql_error(connection) << endl;
        exit(1);
    }

    return mysql_use_result(connection);
}

string connectionDataBase()
{
    MYSQL *con;	// the connection
    MYSQL_RES *numberColumns; //row number
    int columns = 0;
    MYSQL_RES *res;	// the results
    MYSQL_ROW row;	// the results rows (array)

    struct connection_details mysqlD;
    mysqlD.server = "localhost";  // where the mysql database is
    mysqlD.user = "root"; // user
    mysqlD.password = "root"; // the password for the database
    mysqlD.database = "SitioVentas";	// the databse
    

    // connect to the mysql database
    con = mysql_connection_setup(mysqlD);

    string table_name = "PersonaUsuario";
    string queryColumnsNumber = "SELECT count(*) AS NUMBEROFCOLUMNS FROM information_schema.columns WHERE table_name = '";
    string finalQuery = queryColumnsNumber + table_name;
    //Consulta #1
    numberColumns = mysql_perform_query(con, (finalQuery.append("';")).c_str());
    while ((row = mysql_fetch_row(numberColumns)) != NULL){
        // the below row[] parametes may change depending on the size of the table and your objective
        //save number of columns
        columns = *row[0] - '0';
    }
    //cout << "Number of columns: " << *numberRows << endl;
    mysql_free_result(numberColumns);
    

    // get the results from executing commands
    //Consulta #2
    string query = "SELECT * FROM ";
    string data = "";
    res = mysql_perform_query(con, (query.append(table_name)).c_str());
    //cout << ("Database Output:\n") << endl;
    while ((row = mysql_fetch_row(res)) != NULL){
        for(int i = 0; i < columns; i++){
            //cout << row[i] << " | ";
            data += row[i];
            data += " | ";
        }
        data += "\n";
        //cout << typeid(row).name() << endl;
        // the below row[] parametes may change depending on the size of the table and your objective
        
    }

    //cout << "Datos: " << data << endl;
    // clean up the database result
    mysql_free_result(res);

    // close database connection
    mysql_close(con);

    return data;
}

int main(){

	char **qs = web_get_post_string();

	char *user = web_get_from_query_string(qs, "user");
	char *password = web_get_from_query_string(qs, "password");

	web_print_header();

	  
	 cout << "<body>" << endl;

        cout << "<h1>";
        if(sizeof(user)  && sizeof(password)){
            cout << "Usuario:" << user << "<br>";
            cout << "Password:" << password << "<br>";
            cout << sizeof(user)/sizeof(user[0])  << sizeof(password)/sizeof(password[0]) << "<br>";
            cout << sizeof(user[0]) << sizeof(password[0]) << "<br>";
            cout << connectionDataBase() << "<br>";
        }else{
            cout << "No se ha encontrado ningun usuario" << "<br>";
        }
        cout << "</h1>";
	cout << "</body>" << endl;


}//main*/
