#include "sqlite.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataBaseConnection.h"




void tableCreate(){
	init();
	tableCreation();
	db_close();
}


char *  subscribeSubject(int subjectCode, int studentCode){
	char *data;
	char *sql = calloc(1024,sizeof(char));
	int auxiliar;
	init();

//me fijo primero si el cupo no se ocupo
	sprintf(sql, "SELECT CUPO FROM MATERIA WHERE CODIGO = %d", subjectCode);
	data = query(sql);
	
	//printf("AUXILIAR VALEE: ######## %d #####\n", atoi(data));
	if(data == "0"){
		printf("Lo sentimos, esta materia ya agoto su cupo disponible.");
		db_close();
		return "ERROR";
	}

	sprintf(sql,"SELECT COUNT(DISTINCT CODIGO) FROM INSCRIPCION WHERE CODIGO = %d AND LEGAJO = %d", subjectCode,studentCode);
	int num;
	int correlatives;
	data = query(sql);
	auxiliar = atoi(data);

	if(auxiliar!=0){
		printf("ENTRE ACA VIEJITAAAAAA\n");
		printf("Usted ya se inscribio en la materia.");
		db_close();
		return "ERROR";
		
	}
	else{
		sprintf(sql, "SELECT CUPO FROM MATERIA WHERE CODIGO = %d", subjectCode);

		data = query(sql);
		num = atoi(data);

		if(num != 0){
			
			
			sprintf(sql, "SELECT COUNT(*) FROM (SELECT CORRELATIVA_DE FROM CORRELATIVA WHERE CODMATERIA = %d) AS AUXILIAR NATURAL JOIN EXAMEN WHERE AUXILIAR.CORRELATIVA_DE = EXAMEN.CODIGO AND LEGAJO = %d", subjectCode,studentCode);
			data = query(sql);
			correlatives = atoi(data);

			sprintf(sql,"SELECT COUNT(*) FROM CORRELATIVA WHERE CODMATERIA = %d", subjectCode);
			data = query(sql);
			num = atoi(data);
			
			if(correlatives == num){
				sprintf(sql,"INSERT INTO INSCRIPCION(CODIGO, LEGAJO, ANIO) VALUES (%d,%d,2016)" , subjectCode, studentCode );
				data = query(sql);
				sprintf(sql, "UPDATE MATERIA SET CUPO = CUPO - 1 WHERE CODIGO =  %d", subjectCode);
				data = query(sql);

				sprintf(sql, "SELECT CUPO FROM MATERIA WHERE CODIGO = %d", subjectCode);
				data = query(sql);
				printf("DATA ES: %s \n",data );

				db_close();
				return "CORRECTO";

			}else{
				printf("Usted no curso/aprobo las materias correlativas\n");
				db_close();
				return "ERROR";
			}

		}
	}

}


char * cancelSubscription(int subjectCode, int studentCode){
	char *data;
	char *sql = calloc(1024,sizeof(char));
	int num;

	init();
	printf("COMO CODIGO DE MATERIA ME VIENE : %d\n",subjectCode );
	printf("COMO LEGAJO ME VIENE : %d\n",studentCode );

	sprintf(sql, "SELECT COUNT(*) FROM INSCRIPCION WHERE CODIGO = %d AND LEGAJO = %d", subjectCode,studentCode);
	data = query(sql);

	num = atoi(data);

	if(num == 0){
		printf("Usted no se encuentra inscripto en esta materia.\n");
		db_close();
		return "ERROR";
	}else{
		sprintf(sql, "DELETE FROM INSCRIPCION WHERE CODIGO = %d AND LEGAJO = %d", subjectCode,studentCode);
		data = query(sql);
		printf("Usted ha cancelado la subscripcion a la materia de codigo: %d de forma exitosa.", subjectCode);
		sprintf(sql, "UPDATE MATERIA SET CUPO = CUPO  + 1 WHERE CODIGO =  %d", subjectCode);
		data = query(sql);
		db_close();
		return "CORRECTO";
	}

}

char * getSubjectsByCareer(char* careerName){
	char *data;
	char *sql = calloc(1024,sizeof(char));
	int num = -1;

	printf("%s\n",careerName );
	sprintf(sql, "SELECT CODIGO FROM MATERIA WHERE CARRERA = '%s'", careerName);
	data = query(sql);
	
	num = atoi(data);


	printf("valor: %d\n", num);

	if(num == 0){
		printf("Lo sentimos, pero el nombre de la carrera es incorrecto.\n");
		return data;
	}else{
		sprintf(sql, "SELECT NOMBRE FROM MATERIA WHERE CARRERA = '%s'", careerName);
		data = query(sql);
	}

	return data;
}

char * getSubjects(){
	char *data;
	char *sql = calloc(1024, sizeof(char));
	init();

	sprintf(sql, "SELECT CODIGO , NOMBRE FROM MATERIA");
	data = query(sql);
	db_close();
	return data;
}

char * seeCorrelatives(int subjectCode){
	char *data;
	char *sql = calloc(1024,sizeof(char));
	int num;
	init();
	sprintf(sql, "SELECT COUNT(*) FROM MATERIA WHERE CODIGO = %d", subjectCode);
	
	data = query(sql);

	num = atoi(data);
	
	if(num == 0){
		printf("Lo sentimos, pero el codigo de la materia es inexistente.\n");
		return data;
	}else{

		sprintf(sql, "SELECT NOMBRE FROM (SELECT CORRELATIVA_DE from CORRELATIVA where CODMATERIA = %d) , MATERIA WHERE CORRELATIVA_DE = CODIGO  ", subjectCode );
		data = query(sql);
	}
	
	db_close();


	return data;

}


char* getApprovedSubjectsByStudentCode(int studentCode){
	char *data;
	char *sql = calloc(1024,sizeof(char));
	char * carrera;
	char *carrera2= calloc(50,sizeof(char));
	int num;

	sprintf(sql, "SELECT  DISTINCT CARRERA FROM ALUMNO WHERE LEGAJO = %d", studentCode);
	carrera = query(sql);
	strncpy(carrera2, carrera, strlen(carrera)-2);

	sprintf(sql,"SELECT COUNT(*) FROM (SELECT CODIGO, NOMBRE FROM MATERIA WHERE CARRERA = '%s') AUXI , EXAMEN WHERE AUXI.CODIGO = EXAMEN.CODIGO AND NOTA >= 4 AND LEGAJO = %d ", carrera2,studentCode);	
	data = query(sql);
	num = atoi(data);

	if(num == 0){
		printf("Lo sentimos, usted no tiene materias aprobadas.\n");

	}else{
		sprintf(sql,"SELECT NOMBRE, NOTA FROM (SELECT CODIGO, NOMBRE FROM MATERIA WHERE CARRERA = '%s') AUXI , EXAMEN WHERE AUXI.CODIGO = EXAMEN.CODIGO AND NOTA >= 4 AND LEGAJO = %d ", carrera2,studentCode);
		data = query(sql);	
	}
	
	return data;
}


char * getSubjectsByStudentCode(int studentCode){
	char *data;
	char *sql = calloc(1024,sizeof(char));
	char * carrera;
	char *carrera2= calloc(50,sizeof(char));
	int num;

	init();

	sprintf(sql, "SELECT  DISTINCT CARRERA FROM ALUMNO WHERE LEGAJO = %d", studentCode);
	carrera = query(sql);
	strncpy(carrera2, carrera, strlen(carrera)-2);

	sprintf(sql,"SELECT NOMBRE, COALESCE(NOTA, '-') FROM (SELECT CODIGO, NOMBRE FROM MATERIA WHERE CARRERA = '%s') AUXI LEFT OUTER JOIN (SELECT LEGAJO,CODIGO,NOTA FROM EXAMEN WHERE LEGAJO = %d AND NOTA >= 4) AUXI2 ON AUXI.CODIGO = AUXI2.CODIGO", carrera2,studentCode);
	data = query(sql);

	db_close();

	return data;
}

char* getCupo(int subjectCode){
	char *data;
	char *sql = calloc(1024, sizeof(char));
	init();

	sprintf(sql, "SELECT CUPO FROM MATERIA WHERE CODIGO = %d",subjectCode );
	data = query(sql);
	db_close();
	
	return data;	
}



char * checkUser(int username, char * password){
	char * data;
	char *sql = calloc(1024,sizeof(char));
	
	int num;
	init();
	
	sprintf(sql, "SELECT COUNT(*) FROM ALUMNO WHERE LEGAJO = %d", username);
	data = query(sql);
	num = atoi(data);


	if(num == 0){
		printf("Usuario incorrecto.\n");

		return "USUARIO INCORRECTO";
	}else{
		if(checkPassword(username, password) == -1){
			printf("Contrasena incorrecta.\n");
			db_close();
			return "USUARIO INCORRECTO";
		}
	}
	
	db_close();
	return "CORRECTO";
}


int checkPassword(int legajo, char* password){
	char * data;
	char *sql = calloc(1024,sizeof(char));
	int answer;


	sprintf(sql, "SELECT COUNT(*) FROM ALUMNO WHERE LEGAJO = %d AND PASSWORD = '%s'",legajo,password);
	data = query(sql);

	answer = atoi(data);

	if(answer == 0)
		return -1;

	
	return 1;
}
