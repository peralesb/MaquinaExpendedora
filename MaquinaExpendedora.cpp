// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>


using namespace std;

/* By now hardcoded, but can be in a config message file*/
#define MSG_SALUDO		"Bienvenido, introduzca monedas: P Q D N (introduzca \"ok\" para selecionar producto)"
#define ERR_DEN			"Denominacion no aceptada"
#define MSG_CREDITO		"Credito:"
#define MSG_SELECCIONE	"Seleccione producto: A B C"
#define ERR_PRODUCTO	"Producto inexistente"
#define MSG_AGOTADO		"Producto agotado"
#define MSG_ENTREGANDO	"Entregando:"
#define MSG_CAMBIO		"Devolucion"
#define ERR_INSUFICIENTE		"Credito insuficiente"


class Moneda
{
private:
	int cantidad;
	char nombre;
	double denominacion;

public:

	void Init(char n, double d, int c = 100)
	{
		nombre = n;
		denominacion = d;
		cantidad = c;
	}

	/* Por defecto se extrae una moneda*/
	double extrae(int n = 1)
	{
		if (cantidad < n)
			return 0;
		cantidad -= n;
		return denominacion * n;
	}

	char GetNombre()
	{
		return nombre;
	}

	double GetDenominacion()
	{
		return denominacion;
	}
};

class Producto
{
private:
	int cantidad;
	char nombre;
	double costo;

public:

	void Init(char n, double co, int c = 10)
	{
		nombre = n;
		costo = co;
		cantidad = c;
	}

	/* Siempre se extrae un producto (false si no existe)*/
	bool extrae()
	{
		if (!cantidad)
			return false;
		cantidad--;
		return true;
	}

	char GetNombre()
	{
		return nombre;
	}

	double GetPrecio()
	{
		return costo;
	}

};

class Expendedora
{
private:
	vector<Moneda> monedas;
	vector<Producto> productos;

	Moneda P;
	Moneda Q;
	Moneda D;
	Moneda N;

	Producto A;
	Producto B;
	Producto C;

public:

	Expendedora ()
	{
		P.Init('P', 1.0);
		Q.Init('Q', 0.25);
		D.Init('D', 0.1);
		N.Init('N', 0.05);

		monedas.push_back(P);
		monedas.push_back(Q);
		monedas.push_back(D);
		monedas.push_back(N);

		A.Init('A', 0.65);
		B.Init('B', 1.0);
		C.Init('C', 1.5);

		productos.push_back(A);
		productos.push_back(B);
		productos.push_back(C);

	}
	void CompraProducto()
	{
		string entrada;
		int fase = 0;
		double currCred = 0;

		

		while (true)
		{
			vector<Moneda>::iterator iit;
			switch (fase)
			{
			case 0:
				cout << MSG_SALUDO << "\n";
				cin >> entrada;

				if (entrada == "ok")
				{
					/* se acepta el credito actual*/
					fase = 1;
					continue;
				}

				if (entrada.length() > 1 ||
					entrada.length() == 0)

				{
					cout << ERR_DEN << "\n";
					fase = 0;
					continue;
				}

				/* Bucar denominacion*/
				for (iit = monedas.begin(); iit != monedas.end(); iit++)
				{
					/* LA moneda es aceptable ? */
					if (entrada[0] == iit->GetNombre())
					{
						currCred += iit->GetDenominacion();
						cout << MSG_CREDITO << " " << currCred << "\n";
						fase = 0;
						break;
					}
				}

				/*Moneda no aceptable */
				if (iit == monedas.end())
				{
					cout << ERR_DEN << "\n";
					fase = 0;
				}
				continue;

				/* Credito aceptado solicitar producto*/
			case 1:
				if (currCred < 0.65)
				{
					cout << ERR_INSUFICIENTE << "\n";
					fase = 1;
					continue;
				}

				cout << MSG_SELECCIONE << "\n";
				cin >> entrada;

					if (entrada.length() > 1 ||
						entrada.length() == 0)
					{
						cout << ERR_PRODUCTO << "\n";
						fase = 0;
						continue;
					}

					/* Bucar producto*/
					for (vector<Producto>::iterator it = productos.begin(); it != productos.end(); it++)
					{
						double precio = 0;
						/* Producto encontrado*/
						if (entrada[0] == it->GetNombre())
						{
							precio = it->GetPrecio();


							/* Credito insuficiente */
							if (currCred <= precio)
							{
								cout << ERR_INSUFICIENTE << "\n";
								break;
							}

							/*NO agotado*/
							if (it->extrae())
							{
								fase = 2;
								currCred -= precio;
								cout << MSG_ENTREGANDO << it->GetNombre() << "\n";
							}
							/* agotado*/
							else
							{
								cout << MSG_AGOTADO << "\n";
								break;
							}
						}
					}
					continue;
				/*Devolver cambio*/
				case 2:
					string cambio = "";
					cout << "Devolver cambio\n";
					
					for (vector<Moneda>::iterator it = monedas.begin(); it != monedas.end(); it++)
					{
						if (currCred >= it->GetDenominacion())
						{
							cout << " " << it->GetNombre();
							it = monedas.begin();
							currCred -= it->GetDenominacion();
						}
					}
					cout << "\n";

					fase = 0;
					continue;

			}
		}
	}
};


int main()
{
	Expendedora exp;
	while (true)
		exp.CompraProducto();

    return 0;
}

