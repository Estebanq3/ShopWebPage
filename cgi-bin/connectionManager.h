#include <iostream>
#include </usr/include/mysql/mysql.h>
#include <cstring>
#include "connectDataBase.h"
#include "web.h"


using namespace std;

int create_matrix_splitted(string table_name, int colum_number, int row_number, string *ptrMatrix)
{
    int j = 0;
    string articulos_lista = obtain_string_tabledb(table_name, column_number_returner(table_name));
    //cout << articulos_lista << endl;
    //Volvemos el string un char* (mera conversion)
    char* articulos_lista_iterator = &articulos_lista[0];
    char* ptrSplitter = NULL;
    //Hacemos split de los datos pro primera vez, separando en filas distintas cada vez que encuentra un $
    ptrSplitter = strtok(articulos_lista_iterator, "$");

    while(ptrSplitter != NULL)
    {
      //cout << ptrSplitter << endl;

      string line = ptrSplitter;
      string temp = "";
      for(int i=0; i<(int)line.size(); i++){
        // If cur char is not del, then append it to the cur "word", otherwise
          // you have completed the word, print it, and start a new word.
         if(line[i] != '|'){
            temp += line[i];
        }
          else{
            //cout << temp << endl;
            //cout << " J: " << j << endl;
            ptrMatrix[j] = temp;
            temp = "";
            j++;
        }
      }

      ptrSplitter = strtok(NULL, "$");
    }

    return 0;
}



