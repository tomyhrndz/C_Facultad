#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct comp{
    char cod[6];
    int tiempo;
};
struct rec{
    char codep[6];
    char nom[10];
    int time;
};

void bin();
void cargatabla(struct rec a[],int *i);
int buscatabla(struct rec a[],char cod[6],int i);
void imprime();
void opera();
int main()
{
    bin();
    imprime();
    opera();
    return 0;
}
void bin()
{
    FILE *arch1, *arch2, *bin1, *bin2;
    arch1=fopen("competicion.txt","r");
    arch2=fopen("records.txt","r");
    bin1=fopen("bin1.bin","wb");
    bin2=fopen("bin2.bin","wb");
    struct comp a;
    struct rec b;
    if (arch1==NULL || arch2==NULL || bin1==NULL || bin2==NULL)
    {
        perror("error en la carga de archivos en bin\n");
    }
    else
    {
        while(fscanf(arch1,"%s %d",a.cod,&a.tiempo)==2)
        {
            fwrite(&a,sizeof(struct comp),1,bin1);
        }
        while(fscanf(arch2,"%s %s %d",b.codep,b.nom,&b.time)==3)
        {
            fwrite(&b,sizeof(struct rec),1,bin2);
        }
    }
    fclose(arch1);
    fclose(arch2);
    fclose(bin1);
    fclose(bin2);
}
void cargatabla(struct rec a[],int *i)
{
    FILE *bin2;
    bin2=fopen("bin2.bin","rb");
    (*i)=0;
    if (bin2==NULL)
    {
        perror("error en la carga de archivos en cargatabla\n");
    }
    else
    {
        while (fread(&a[*i],sizeof(struct rec),1,bin2))
        {
            (*i)++;
        }
    }
    fclose(bin2);
}
int buscatabla(struct rec a[],char cod[6],int i)
{
    int j=0;
    while((strcmp(a[j].codep,cod)!=0) && j<i)
    {
        j++;
    }
    return j;
}
void imprime()
{
    printf("DEPORTE\t\tCANTCOMP\tCANTSUP\tPROM TIEMP\n");
}
void opera()
{
    FILE *bin1;
    bin1=fopen("bin1.bin","rb");
    struct rec mat[10];
    int i,pos,cont,sup,t;
    float prom;
    struct comp b;
    char aux[6];
    if (bin1==NULL)
    {
        perror("error en la carga de archivos en opera\n");
    }
    else
    {
        fread(&b,sizeof(struct comp),1,bin1);
        cargatabla(mat,&i);
        while(!feof(bin1))
        {
            t=0;
            prom=0;
            cont=0;
            sup=0;
            pos=buscatabla(mat,b.cod,i);
            //printf("%d",pos);
            strcpy(aux,b.cod);
            while(!feof(bin1) && (strcmp(aux,b.cod)==0))
            {
               if (b.tiempo>=mat[pos].time)
               {
                   sup++;
               }
               t+=b.tiempo;
               cont++;
               fread(&b,sizeof(struct comp),1,bin1);
            }
            prom=(float)t/cont;
            printf("%s\t %d \t %d \t %.2f\n",mat[pos].nom,cont,sup,prom);
        }
    }
    fclose(bin1);
}