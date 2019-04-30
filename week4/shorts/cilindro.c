#include <stdio.h>
#include <cs50.h>

void VyA(double* D, double* H, double* v, double* a); //Volumen y Area

void Opti(double* Vo, double* Do, double* Ho);

int main (void)
{
	double d, h, V, A;

	printf("Si desea calcular el Volumen del cilindro y su uptimizacion a paritr del diametro y la altura, presione 0 + enter\n");
	printf("Si desea a partir de un volumen calcular el diámetro y la altura optima del cilindro, presioe cualquier otro numero + enter: ");
	int i = GetInt();
	if (i == 0)
	{
		do
		{
			printf("Ingrese el diámetro en centímetros del cilindro entre [1; 50]cm : ");
			d = GetDouble();
		}
		while (d >= 50 && d <= 1);
		do
		{
			printf("Ingrese la altura en centímetros del cilindro entre [1; 50]cm : ");
			h = GetDouble();
		}
		while (h >= 50 && h <= 1);
		VyA(&d, &h, &V, &A);
		double A1 = A;
		printf("El cilindro tiene un volumen de: %.2lfml\n", V);
		printf("El cilindro tiene un Area de: %.2lfcm2\n", A);
		Opti(&V, &d, &h);
		printf("EL diametro que optimiza el volumen calculado del cilindro es: %.2lfcm y la altura es: %.2lfcm\n", d, h);
		VyA(&d, &h, &V, &A);
		printf("El Area total del cilindro para esas medidas optimizadas es de: %.2lfcm2\n", A);
		printf("Y el Area de material que se ahorra es de: %.2lfcm2 o en porcentaje: %.2lf%%\n", A1 - A, ((A1-A)/A1)*100);
		return 0;
	}
	else
	{
		do
		{
			printf("Ingrese el la cantidad de ml en el cilindro entre [50; 10000]ml3 : ");
			V = GetDouble();
		}
		while (V >= 10000 && V <= 50);
		Opti(&V, &d, &h);
		printf("EL diametro que optimiza el volumen calculado del cilindro es: %.2lfcm y la altura es: %.2lfcm\n", d, h);
		VyA(&d, &h, &V, &A);
		printf("Y el Area total del cilindro para esas medidas optimizadas es de: %.2lfcm2\n", A);
		return 0;
	}

}

void VyA(double* D, double* H, double* v, double* a)
{
	*v = 3.14*(*D)*(*D)*(*H)/4.0;
	*a = 3.14*(*D)*(*D)/2.0 + 3.14*(*D)*(*H);
}

void Opti(double* Vo, double* Do, double* Ho) //Volumen Optimo, Diametro Optimo, Longitud Optima
{
	double Op = (*Vo)/(2.0*3.14);
	(*Do) = Op;
	while ((Op*Op*Op) > (*Do))
		Op = Op/2.0;
	while ((Op*Op*Op) < (*Do))
		Op = Op + 0.10;
	while ((Op*Op*Op) > (*Do))
		Op = Op - 0.01;
	while ((Op*Op*Op) < (*Do))
		Op = Op + 0.001;
	*Do = Op*2; //lo que me da es el radio, por lo que esto me dara el diametro
	*Ho = *Do;
}

