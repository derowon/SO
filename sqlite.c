#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite.h"

sqlite3 *db;
int rc;
char * zErrorMsg = 0;

//Database initialization
void init(){

  
  
  rc = sqlite3_open("sga.db",&db);

  if(rc){
    fprintf(stderr,"Can't open database: %s \n",sqlite3_errmsg(db));
    exit(0);
  }else{
    fprintf(stderr,"Opened database successfully\n");
    printf("EL VALOR DE RC ES: %d\n", rc);
  }
 //tableCreation();


}

void close(){
  sqlite3_close(db);
}

//Database management
/* int main(){

  init();
  

  sqlite3_close(db);

}*/




static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      //printf("HOLA CHINITO\n");

      //if(*(char*)data != NULL)
        sprintf(data + strlen(data), "%s\t" , argv[i] ? argv[i] : "NULL");
   }
   

   //if(*(char*)data != NULL)
      sprintf(data + strlen(data), "\n");

   return 0;
}

char * query(char * sql){
  
  char * data=calloc(3000,sizeof(char));

  rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrorMsg);
  if( rc != SQLITE_OK ){
  fprintf(stderr, "SQL error: %s\n", zErrorMsg);
     sqlite3_free(zErrorMsg);
  }else{
     fprintf(stdout, "SQL with success\n");
  }



  return data;
}



void tableCreation(){

  char * sql;


  /* Create SQL statement */
  /*Creo la tabla Carrera*/
  sql = "CREATE TABLE CARRERA("  \
        "NOMBRE TEXT     NOT NULL," \
        "CREDITOS INT    NOT NULL," \

        "PRIMARY KEY(NOMBRE)"
        ");";
  query(sql); //1

  /*Creo la tabla Materias*/
   sql = "CREATE TABLE MATERIA("  \
         "CODIGO INT     NOT NULL," \
         "NOMBRE           TEXT    NOT NULL," \
         "CUPO            INT     NOT NULL," \
         "CARRERA  TEXT   ," \

         "PRIMARY KEY(CODIGO),"\
         "FOREIGN KEY(CARRERA) REFERENCES CARRERA ON DELETE SET NULL"
         ");";

  /* Execute SQL statement */
    query(sql); //2


  /* Create SQL statement */
     sql = "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA) "  \
           "VALUES (10, 'Sistemas de Representacion', 20,'INFORMATICA'); " \
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA) "  \
           "VALUES (20, 'Introduccion a la Informatica', 25,'INFORMATICA'); "     \
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (30, 'Analisis Matematico I', 23,'INFORMATICA');" \
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (40, 'Algebra', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (50, 'Metodologia del Aprendizaje', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (60, 'Programacion Imperativa', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (70, 'Analisis Matematico II', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (80, 'Fisica I', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (90, 'Matematica Discreta', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (100, 'Quimica', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (110, 'Diseno y procesamiento de doc XML', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (120, 'Programacion orientada a objetos', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (130, 'Logica Computacional', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (140, 'Fisica', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (150, 'Arquitectura de Computadoras', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (160, 'Estructura de Datos y Algoritmos', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (170, 'Probabilidad y Estadistica', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (180, 'Fisica II', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (190, 'Sistemas Operativos', 25,'INFORMATICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (11, 'Introduccion a la computacion', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (22, 'Matematica I', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (33, 'Algebra Lineal', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (44, 'Programacion I', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (55, 'Matematica II', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (66, 'Matematica III', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (77, 'Algoritmos', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (88, 'Tecnologia de materiales electronicos', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (99, 'Matematica IV', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (111, 'Organizacion Industrial', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (122, 'Electrotenia I', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (133, 'Matematica IV', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (144, 'Teoria de circuitos', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (155, 'Electrotenia III', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (166, 'Laboratorio de Electrotenia', 25,'ELECTRONICA');"\
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (177, 'Electronica I', 25,'ELECTRONICA');";


  /* Execute SQL statement */
    query(sql); //3

    
   

   /*Creo la tabla Alumnos*/
   sql = "CREATE TABLE ALUMNO("  \
         "LEGAJO INT      NOT NULL," \
         "NOMBRE           TEXT    NOT NULL," \
         "SEXO            TEXT     NOT NULL," \
         "PASSWORD           TEXT     NOT NULL," \
         "CARRERA            TEXT     NOT NULL" \
         ");";

   /* Execute SQL statement */      
   query(sql); //4

 /* Create SQL statement */
   sql = "INSERT INTO ALUMNO (LEGAJO,NOMBRE,SEXO,PASSWORD,CARRERA) "  \
           "VALUES (52000, 'Won, Dae Ro', 'M', 'dwon', 'INFORMATICA'); "\
           "INSERT INTO ALUMNO (LEGAJO,NOMBRE,SEXO,PASSWORD,CARRERA) "  \
           "VALUES (5400, 'Guido, Sebastian Ezequiel', 'M', 'sguido','INFORMATICA');"\
           "INSERT INTO ALUMNO (LEGAJO,NOMBRE,SEXO,PASSWORD,CARRERA) "  \
           "VALUES (53384, 'Castano, Nicolas Martin', 'M', 'ncastano','INFORMATICA');";


   /* Execute SQL statement */
    query(sql); //5

   /*Creo la tabla Inscripcion*/
  sql = "CREATE TABLE INSCRIPCION("  \
         "CODIGO INT     NOT NULL," \
         "LEGAJO    TEXT    NOT NULL," \
         "ANIO            DATE     NOT NULL," \

         "PRIMARY KEY(CODIGO, LEGAJO, ANIO), "\
         "FOREIGN KEY(CODIGO) REFERENCES MATERIA ON DELETE CASCADE,"\
         "FOREIGN KEY(LEGAJO) REFERENCES ALUMNO ON DELETE CASCADE"\
         ");";

    /* Execute SQL statement */
    query(sql); //6

    /* Create SQL statement */
   sql = "INSERT INTO INSCRIPCION (CODIGO,LEGAJO, ANIO) "  \
           "VALUES (1, 1, 2016);"\
           "INSERT INTO INSCRIPCION (CODIGO,LEGAJO, ANIO) "  \
           "VALUES (1, 2, 2016);";

    /* Execute SQL statement */
    query(sql);


  /*Creo la tabla Profesor*/
   sql = "CREATE TABLE PROFESOR("
         "LEGPROF INT     NOT NULL," \
         "NOMBRE    TEXT    NOT NULL," \
         "ANTIGUEDAD  DATE     NOT NULL," \
         "PRIMARY KEY(LEGPROF) "\
         ");";
  /* Execute SQL statement */     
  query(sql); //7


  /*Creo la tabla Examen*/
     sql = "CREATE TABLE EXAMEN("
           "LEGAJO INT     NOT NULL," \
           "LEGPROF    INT    NOT NULL," \
           "NOTA  INT     NOT NULL," \
           "FECHA  DATE     NOT NULL," \
           "CODIGO  INT     NOT NULL," \
           "NROACTA  INT     NOT NULL," \
           "PRIMARY KEY(LEGAJO, LEGPROF, FECHA, CODIGO), "\
           "FOREIGN KEY(LEGAJO) REFERENCES ALUMNO ON DELETE CASCADE,"\
           "FOREIGN KEY(LEGPROF) REFERENCES PROFESOR ON DELETE CASCADE,"\
           "FOREIGN KEY(CODIGO) REFERENCES MATERIA ON DELETE CASCADE"\
           ");";


    /* Execute SQL statement */
    query(sql); //8

    /* Create SQL statement */
    sql = "INSERT INTO EXAMEN (LEGAJO,LEGPROF,NOTA,FECHA,CODIGO,NROACTA) "  \
           "VALUES (1, 1, 10, 2016-11-30, 1, 123); "\
           "INSERT INTO EXAMEN (LEGAJO,LEGPROF,NOTA,FECHA,CODIGO,NROACTA) "  \
           "VALUES (2, 1, 3, 2016-11-30, 1, 123);";
    query(sql); //9

    /*Creo la tabla Correlativa*/
    sql = "CREATE TABLE CORRELATIVA("
           "CODMATERIA INT     NOT NULL," \
           "CORRELATIVA_DE    INT    NOT NULL," \
           
           "PRIMARY KEY(CODMATERIA,CORRELATIVA_DE), "\
           "FOREIGN KEY(CODMATERIA) REFERENCES MATERIA ON DELETE CASCADE"\
           ");";

    /* Execute SQL statement */
    query(sql); //10


     /* Execute SQL statement */
    sql = "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE) "  \
           "VALUES (60,20); "\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (60,40);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (140,80);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (120,60);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (120,20);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (120,40);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (150,60);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (150,20);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (150,40);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (150,120);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (160,120);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (160,60);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (160,20);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (160,40);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (190,150);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (190,160);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (190,120);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (190,60);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (190,20);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (190,40);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (44,11);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (44,33);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (55,22);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (66,55);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (66,22);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (77,44);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (77,11);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (77,33);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (88,77);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (88,44);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (88,33);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (88,11);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (99,66);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (99,55);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (99,22);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (133,99);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (133,66);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (133,55);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (133,22);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (155,122);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (177,122);";

   // printf("HOLA\n");
    /* Execute SQL statement */
    query(sql); //11

}



