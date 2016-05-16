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


}

void close(){
  sqlite3_close(db);
}






static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      
        sprintf(data + strlen(data), "%s\t" , argv[i] ? argv[i] : "NULL");
   }
   

   
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
           "VALUES (1, 'A', 20,'INFORMATICA'); " \
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA) "  \
           "VALUES (2, 'B', 25,'INFORMATICA'); "     \
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (3, 'C', 23,'QUIMICA');" \
           "INSERT INTO MATERIA (CODIGO,NOMBRE,CUPO,CARRERA)" \
           "VALUES (4, 'D', 25,'INDUSTRIAL');";

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
           "VALUES (1, 'DERO', 'M', '1234', 'INFORMATICA'); "\
           "INSERT INTO ALUMNO (LEGAJO,NOMBRE,SEXO,PASSWORD,CARRERA) "  \
           "VALUES (2, 'NICO', 'M', '1234','INFORMATICA');";

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
           "VALUES (3,2); "\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (2,1);"\
           "INSERT INTO CORRELATIVA (CODMATERIA, CORRELATIVA_DE)"  \
           "VALUES (3,1);";

  
    /* Execute SQL statement */
    query(sql); //11

}



