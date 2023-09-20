#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct datos{
    char CodE[4];
    float ton;
};
struct Cupos{
    char CodEM[5];
    struct datos cups;
};
struct Capturas{
    char CodEmp[5];
    char CodB[4];
    struct datos capt;
};
struct Multas{
    char CodEmpres[5];
    struct datos mult;
};
void opera();
void imprime();
int main()
{
    opera();
    imprime();
}
void opera()
{
    FILE *bin1, *bin2, *bin3;
    bin1=fopen("Cupos.bin","rb");
    bin2=fopen("Capturas.bin","rb");
    bin3=fopen("bin3.bin","wb");
    struct Cupos a;
    struct Capturas b;
    struct Multas c;
    float aux;
    int cont;//respeto el cupo
    if(bin1==NULL || bin2==NULL)
    {
        perror("Error en la carga de archivos de opera");
    }
    else
    {
        cont=0;
        fread(&a,sizeof(struct Cupos),1,bin1);
        fread(&b,sizeof(struct Capturas),1,bin2);
        while(!feof(bin1) && !feof(bin2))
        {
            if(a.CodEM==b.CodEmp)
            {
                if(a.cups.CodE==b.capt.CodE)
                {
                    //pesco lo que debia
                    if(a.cups.ton<=b.capt.ton)
                    {
                        //no se paso del limite
                        fread(&a,sizeof(struct Cupos),1,bin1);
                        fread(&b,sizeof(struct Capturas),1,bin2);
                        cont++;
                    }
                    else
                    {
                        //se paso del limite
                        strcpy(c.CodEmpres,a.CodEM);
                        strcpy(c.mult.CodE,a.cups.CodE);
                        aux=a.cups.ton-b.capt.ton;
                        c.mult.ton=aux;
                        fwrite(&c,sizeof(struct Multas),1,bin3);
                        fread(&a,sizeof(struct Cupos),1,bin1);
                        fread(&b,sizeof(struct Capturas),1,bin2);
                    }
                }
                else
                {
                    //no pesco lo que debia
                    strcpy(c.CodEmpres,a.CodEM);
                    strcpy(c.mult.CodE,a.cups.CodE);
                    aux=a.cups.ton-b.capt.ton;
                    c.mult.ton=aux;
                    fwrite(&c,sizeof(struct Multas),1,bin3);
                    fread(&a,sizeof(struct Cupos),1,bin1);
                    fread(&b,sizeof(struct Capturas),1,bin2);
                }
            }
            else
                if(a.CodEM<b.CodEmp)
                {
                    //no autorizado
                    strcpy(c.CodEmpres,a.CodEM);
                    strcpy(c.mult.CodE,a.cups.CodE);
                    c.mult.ton=a.cups.ton;
                    fwrite(&c,sizeof(struct Multas),1,bin3);
                    fread(&a,sizeof(struct Cupos),1,bin1);
                }
                else
                {
                    //esta hablitado y no mando barco
                    fread(&b,sizeof(struct Capturas),1,bin2);
                    cont++;
                }
        }

    }
    printf("%d empresas respetaron sus cupos",cont);
    fclose(bin1);
    fclose(bin2);
    fclose(bin3);
}
void imprime()
{
    FILE *bin3;
    bin3=fopen("bin3.bin","rb");
    struct Multas a;
    if(bin3==NULL)
    {
        perror("Error en la carga de archivos de imprime");
    }
    else
    {
        while(fread(&a,sizeof(struct Multas),1,bin3))
        {
            printf("%s %s %f\n",a.CodEmpres,a.mult.CodE,a.mult.ton);
        }
    }
    fclose(bin3);
}
