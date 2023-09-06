#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct horaE{
    char fecha[7];
    int leg;
    char horteo[6];
};
struct reloj{
    char fecha[7];
    int leg;
    char horareal[6];
};
struct final{
    char fecha[7];
    int leg;
    char horteo[6];
    char horreal[6];
    char estado[2];
};
void bin();
void opera();
void imprimir();
int main()
{
    bin();
    opera();
    imprimir();
    return 0;
}
void bin()
{
    FILE *arch1,*arch2,*bin1,*bin2;
    arch1=fopen("HoraEntPersonal.txt","r");
    arch2=fopen("RelojEntrada.txt","r");
    bin1=fopen("bin1.bin","wb");
    bin2=fopen("bin2.bin","wb");

    struct horaE a;
    struct reloj b;
    
    if (arch1==NULL || arch2==NULL || bin1==NULL || bin2==NULL)
    {
        perror("Error en la carga de archivos en bin");
    }
    else 
    {
        while (fscanf(arch1,"%s %d %s",a.fecha,&a.leg,a.horteo)==3)
        {
            fwrite(&a,sizeof(struct horaE),1,bin1);
            //printf("%s\t%d\t%s\n",a.fecha,a.leg,a.horteo);
        }
        while (fscanf(arch2,"%s %d %s",b.fecha,&b.leg,b.horareal)==3)
        {
            fwrite(&b,sizeof(struct reloj),1,bin2);
            //printf("%s\t%d\t%s\n",b.fecha,b.leg,b.horareal);
        }
    }
    fclose(arch1);
    fclose(arch2);
    fclose(bin1);
    fclose(bin2);
}
void opera()
{
    FILE *bin1,*bin2,*bin3;
    bin1=fopen("bin1.bin","rb");
    bin2=fopen("bin2.bin","rb");
    bin3=fopen("bin3.bin","wb");
    struct horaE a;
    struct reloj b;
    struct final c;
    int contA=0;
    int cont=0;
    int contT=0;
    int error=0;
    float porcA=0;
    if (bin1==NULL || bin2==NULL || bin3==NULL)
    {
        perror("Error en la carga de archivos en opera");
    }
    else
    {
        fread(&a,sizeof(struct horaE),1,bin1);
        fread(&b,sizeof(struct reloj),1,bin2);
        while(!feof(bin1))
        {
            if(strcmp(a.fecha,b.fecha)==0)
            {
                if(a.leg==b.leg)
                {
                    if((strcmp(a.horteo,b.horareal)==0) || (strcmp(a.horteo,b.horareal)>0))
                    {
                        //entro a tiempo
                        strcpy(c.fecha,a.fecha);
                        c.leg=a.leg;
                        strcpy(c.horteo,a.horteo);
                        strcpy(c.horreal,b.horareal);
                        strcpy(c.estado,"P");
                        fwrite(&c,sizeof(struct final),1,bin3);
                        fread(&a,sizeof(struct horaE),1,bin1);
                        fread(&b,sizeof(struct reloj),1,bin2);
                    }
                    else
                        if (strcmp(a.horteo,b.horareal)<0)
                        {
                            //llego tarde
                            strcpy(c.fecha,a.fecha);
                            c.leg=a.leg;
                            strcpy(c.horteo,a.horteo);
                            strcpy(c.horreal,b.horareal);
                            strcpy(c.estado,"T");
                            contT++;
                            fwrite(&c,sizeof(struct final),1,bin3);
                            fread(&a,sizeof(struct horaE),1,bin1);
                            fread(&b,sizeof(struct reloj),1,bin2);
                        }
                }
                else
                    if (a.leg<b.leg)
                    {
                        //falto
                        strcpy(c.fecha,a.fecha);
                        c.leg=a.leg;
                        strcpy(c.horteo,a.horteo);
                        strcpy(c.horreal,"\0");
                        strcpy(c.estado,"A");
                        contA++;
                        fwrite(&c,sizeof(struct final),1,bin3);
                        fread(&a,sizeof(struct horaE),1,bin1);
                    }
                    else
                        if (a.leg>b.leg)
                        {
                          //ficha erronea
                          error++;
                          fread(&b,sizeof(struct reloj),1,bin2);
                        }
            }
            else
                if(strcmp(a.fecha,b.fecha)<0)
                {
                    //falto
                    strcpy(c.fecha,a.fecha);
                    c.leg=a.leg;
                    strcpy(c.horteo,a.horteo);
                    strcpy(c.horreal,"\0");
                    strcpy(c.estado,"A");
                    contA++;
                    fwrite(&c,sizeof(struct final),1,bin3);
                    fread(&a,sizeof(struct horaE),1,bin1);
                }
                else
                    if(strcmp(a.fecha,b.fecha)>0)
                    {
                        //ficha erronea
                        error++;
                        fread(&b,sizeof(struct reloj),1,bin2);
                    }
                    cont++;
        }
        cont-=error;
        cont--;
        porcA=(float)contA*100/cont;
        printf("El porcentaje de ausentismo es del %%%.2f\n",porcA);
        printf("Hubo %d entradas demoradas\n",contT);
        printf("Hubo %d fichadas erroneas\n\n",error);
    }
    fclose(bin1);
    fclose(bin2);
    fclose(bin3);
}
void imprimir()
{
    FILE *bin3;
    bin3=fopen("bin3.bin","rb");
    struct final a;
    if (bin3==NULL)
    {
        perror("Error en la carga de archivos de imprime");
    }
    else
    {
        while(fread(&a,sizeof(struct final),1,bin3))
        {
            printf("%s\t%d\t%s\t%s\t%s\n",a.fecha,a.leg,a.horteo,a.horreal,a.estado);
        }
    }
    fclose(bin3);
}
