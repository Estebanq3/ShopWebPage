PruebaMetodosBaseDatos: pruebaMetodosBase.cpp
	g++ -std=c++0x -o pruebaMetodosBase pruebaMetodosBase.cpp Hash.cpp `mysql_config --cflags --libs` -lcryptopp -lgcrypt && ./pruebaMetodosBase
PaginaRegistro: register_page.cpp	
	g++ -std=c++0x -o register_page.cgi register_page.cpp Hash.cpp `mysql_config --cflags --libs` -lcryptopp -lgcrypt
PaginaLogin: login_page.cpp
	g++ -std=c++0x -o login_page.cgi login_page.cpp Hash.cpp `mysql_config --cflags --libs` -lcryptopp -lgcrypt
PaginaShopCart: shoppingcart_page.cpp
	g++ -std=c++0x -o shoppingcart_page.cgi shoppingcart_page.cpp Hash.cpp `mysql_config --cflags --libs` -lcryptopp -lgcrypt
PaginaPayment: paymentMethod.cpp
	g++ -std=c++0x -o paymentMethod_page.cgi paymentMethod.cpp Hash.cpp  `mysql_config --cflags --libs` -lcryptopp -lgcrypt