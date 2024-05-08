#include <stdio.h>

int contarDigitos(int numero)
{
    int contador = 0;
    while (numero != 0)
    {
        numero /= 10; // Dividimos el número por 10 para eliminar el último dígito
        contador++;   // Incrementamos el contador
    }
    return contador - 1;
}

void obtenerNumSinPrimerDigito(int arreglo[], int num, int longitud)
{

    for (int i = longitud - 1; i >= 0; i--)
    {
        arreglo[i] = num % 10; // Obtener el último dígito y almacenarlo en el vector
        num /= 10;             // Eliminar el último dígito del número
    }
}

int decimal_a_binario(int numeroDecimal)
{
    int binario = 0;
    int peso = 1; // Peso del bit actual
    int positivoNumDecimal = numeroDecimal;

    if (positivoNumDecimal < 0)
    {
        positivoNumDecimal *= -1;
    }
    // Proceso de división sucesiva por 2 para la conversión a binario
    while (positivoNumDecimal != 0)
    {
        binario += (positivoNumDecimal % 2) * peso; // Agregar el bit al número binario
        positivoNumDecimal /= 2;                    // Dividir el número decimal por 2
        peso *= 10;                                 // Incrementar el peso del bit
    }

    return binario;
}

void armarArreglo(int arreglo[32], int numeroDecimal, int numeroSinPrimerNumero[], int binarioSumatoriaDigitos, int digitos)
{

    // lenando el primer espacio del vector
    if (numeroDecimal > 0)
    {
        arreglo[0] = 0;
    }
    else
    {
        arreglo[0] = 1;
    }

    for (int i = 8; i >= 1; i--)
    {
        arreglo[i] = binarioSumatoriaDigitos % 10; // Obtiene el último dígito
        binarioSumatoriaDigitos /= 10;             // Elimina el último dígito
    }

    for (int i = 0, j = 9; i < digitos; i++, j++)
    {
        arreglo[j] = numeroSinPrimerNumero[i];
    }

    for (int i = (9 + digitos); i < 32; i++)
    {
        arreglo[i] = 0;
    }
}

void obtenerResultadoComaFlotante(int numeroDecimal, int numeroCompleto[32])
{
    int numeroBinario = decimal_a_binario(numeroDecimal); // obtenemos el numero binario
    // printf("\n\nEl numero binario del numero decimal %d es %d",numeroDecimal,numeroBinario);

    int digitos = contarDigitos(numeroBinario); // contamos la longitud del numero binario y le restamos 1
    // printf("\n\nLa cantidad de digitos es %d", digitos);

    int numeroSinPrimerNumero[digitos]; // vector donde guardaremos el numero binario sin el primer digito
    obtenerNumSinPrimerDigito(numeroSinPrimerNumero, numeroBinario, digitos);
    // printf("\n\nMostrando el binario en formato vector: ");
    for (int i = 0; i < digitos; i++)
    {
        //    printf("%d", numeroSinPrimerNumero[i]);
    }

    int sumatoriaDigitos = digitos + 127;
    // printf("\n\nSumamos 127 mas los digitos del codigo binario %d = %d ",digitos, sumatoriaDigitos);

    int binarioSumatoriaDigitos = decimal_a_binario(sumatoriaDigitos);
    // printf("\n\nEl numero binario del numero %d es %d",sumatoriaDigitos,  binarioSumatoriaDigitos);

    armarArreglo(numeroCompleto, numeroDecimal, numeroSinPrimerNumero, binarioSumatoriaDigitos, digitos);
}

void mostrarResultado(int vector[32])
{
    for (int i = 0; i < 32; i++)
    {
        printf("%d", vector[i]);
    }
}

void sumar(int numero1, int numero2)
{

    int resultado = numero1 + numero2;
    int numeroCompleto[32];

    printf("El resultado de la suma en decimal es: %d\n", resultado);
    printf("El resultado de la suma en hexadecimal es: %X", resultado);
    obtenerResultadoComaFlotante(resultado, numeroCompleto);
    printf("\nMostrando el resultado en coma flotante: ");
    mostrarResultado(numeroCompleto);
}

void restar(int numero1, int numero2)
{

    int resultado = numero1 - numero2;
    int numeroCompleto[32];

    printf("El resultado de la resta en decimal es: %d\n", resultado);
    printf("El resultado de la resta en hexadecimal es: %X", resultado);
    obtenerResultadoComaFlotante(resultado, numeroCompleto);
    printf("\nMostrando el resultado en coma flotante: ");
    mostrarResultado(numeroCompleto);
}

void multiplicar(int numero1, int numero2)
{

    int resultado = numero1 * numero2;
    int numeroCompleto[32];

    printf("El resultado de la multiplicacion en decimal es: %d\n", resultado);
    printf("El resultado de la multiplicacion en hexadecimal es: %X", resultado);
    obtenerResultadoComaFlotante(resultado, numeroCompleto);
    printf("\nMostrando el resultado en coma flotante: ");
    mostrarResultado(numeroCompleto);
}

void dividir(int numero1, int numero2)
{

    int resultado = numero1 / numero2;
    int numeroCompleto[32];

    printf("El resultado de la multiplicacion en decimal es: %d\n", resultado);
    printf("El resultado de la multiplicacion en hexadecimal es: %X", resultado);
    obtenerResultadoComaFlotante(resultado, numeroCompleto);
    printf("\nMostrando el resultado en coma flotante: ");
    mostrarResultado(numeroCompleto);
}

void operacionBitAnd(int numeroCompleto1[32], int numeroCompleto2[32])
{
    int numeroCompletoResultado[32];
    for (int i = 0; i < 32; i++)
    {
        numeroCompletoResultado[i] = (numeroCompleto1[i] && numeroCompleto2[i]);
    }
    printf("\n----------------------------------------------------------");
    printf("\n\nMostrando el resultado de la operacion And\n");
    mostrarResultado(numeroCompletoResultado);
    printf("\n");
}

void operacionBitOr(int numeroCompleto1[32], int numeroCompleto2[32])
{
    int numeroCompletoResultado[32];
    for (int i = 0; i < 32; i++)
    {
        numeroCompletoResultado[i] = (numeroCompleto1[i] || numeroCompleto2[i]);
    }
    printf("\n----------------------------------------------------------");
    printf("\n\nMostrando el resultado de la operacion Or\n");
    mostrarResultado(numeroCompletoResultado);
    printf("\n");
}

void operacionBitXor(int numeroCompleto1[32], int numeroCompleto2[32])
{
    int numeroCompletoResultado[32];
    for (int i = 0; i < 32; i++)
    {
        numeroCompletoResultado[i] = (numeroCompleto1[i] != numeroCompleto2[i]);
    }
    printf("\n----------------------------------------------------------");
    printf("\n\nMostrando el resultado de la operacion XOR\n");
    mostrarResultado(numeroCompletoResultado);
    printf("\n");
}

void operacionBitNot(int numeroCompleto1[32], int numeroCompleto2[32])
{
    int numeroCompletoResultado1[32];
    int numeroCompletoResultado2[32];
    for (int i = 0; i < 32; i++)
    {
        numeroCompletoResultado1[i] = !numeroCompleto1[i];
        numeroCompletoResultado2[i] = !numeroCompleto2[i];
    }
    printf("\n----------------------------------------------------------");
    printf("\n\nMostrando el resultado de la operacion NOT para el primer numero\n");
    mostrarResultado(numeroCompletoResultado1);

    printf("\n\nMostrando el resultado de la operacion NOT para el segundo numero\n");
    mostrarResultado(numeroCompletoResultado2);
    printf("\n");
}

void operacionBitNand(int numeroCompleto1[32], int numeroCompleto2[32])
{
    int numeroCompletoResultado[32];
    for (int i = 0; i < 32; i++)
    {
        numeroCompletoResultado[i] = !(numeroCompleto1[i] && numeroCompleto2[i]);
    }
    printf("\n----------------------------------------------------------");
    printf("\n\nMostrando el resultado de la operacion NAND\n");
    mostrarResultado(numeroCompletoResultado);
    printf("\n");
}

void operacionBitNor(int numeroCompleto1[32], int numeroCompleto2[32])
{
    int numeroCompletoResultado[32];
    for (int i = 0; i < 32; i++)
    {
        numeroCompletoResultado[i] = !(numeroCompleto1[i] || numeroCompleto2[i]);
    }
    printf("\n----------------------------------------------------------");
    printf("\n\nMostrando el resultado de la operacion NOR\n");
    mostrarResultado(numeroCompletoResultado);
    printf("\n");
}

void operacionBitXnor(int numeroCompleto1[32], int numeroCompleto2[32])
{
    int numeroCompletoResultado[32];
    for (int i = 0; i < 32; i++)
    {
        numeroCompletoResultado[i] = !(numeroCompleto1[i] != numeroCompleto2[i]);
    }
    printf("\n----------------------------------------------------------");
    printf("\n\nMostrando el resultado de la operacion XNOR\n");
    mostrarResultado(numeroCompletoResultado);
    printf("\n");
}

int main()
{
    int numero1, numero2;
    int opc, res;
    int numeroFlotante1[32];
    int numeroFlotante2[32];

    do
    {

        printf("Introduce el primer numero: ");
        scanf("%d", &numero1);

        printf("Introduce el segundo numero: ");
        scanf("%d", &numero2);

        printf("\nMENU DE OPCIONES:\n1-SUMAR\n2-RESTAR\n3-MULTIPLICAR\n4-DIVIDIR\n5-OPERACIONES LOGICAS\n6-SALIR ");
        scanf("%d", &opc);

        if (numero1 == 0 || numero2 == 0 && opc == 3)
        {
            printf("El resultado en decimal es: 0\n");
            printf("El resultado en hexadecimal es: 0\n");
        }
        else
        {
            switch (opc)
            {

            case 1:
                sumar(numero1, numero2);
                break;

            case 2:
                restar(numero1, numero2);
                break;

            case 3:
                multiplicar(numero1, numero2);
                break;

            case 4:
                while (numero2 == 0 || numero1 < numero2)
                {
                    printf("Introduce el segundo numero diferente de cero y menor que el primer numero para poder ejecutar la division: ");
                    scanf("%d", &numero2);
                }
                dividir(numero1, numero2);
                break;

            case 5:
                obtenerResultadoComaFlotante(numero1, numeroFlotante1);
                printf("\nMostrando el coma flotante 1\n");
                mostrarResultado(numeroFlotante1);

                obtenerResultadoComaFlotante(numero2, numeroFlotante2);
                printf("\n\nMostrando el coma flotante 2\n");
                mostrarResultado(numeroFlotante2);

                operacionBitAnd(numeroFlotante1, numeroFlotante2);
                operacionBitOr(numeroFlotante1, numeroFlotante2);
                operacionBitXor(numeroFlotante1, numeroFlotante2);
                operacionBitNot(numeroFlotante1, numeroFlotante2);
                operacionBitNand(numeroFlotante1, numeroFlotante2);
                operacionBitNor(numeroFlotante1, numeroFlotante2);
                operacionBitXnor(numeroFlotante1, numeroFlotante2);
                break;

            case 6:
                printf("\nHASTA LUEGO");
                break;

            default:
                printf("\nOPCION INVALIDA");
            }
        }

        printf("\n\nDesea continuar en el programa? 1-Si/0-No: ");
        scanf("%d", &res);

    } while (res == 1);

    return 0;
}