#include "sqlite.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkPassword(int legajo, char* password);

int  subscribeSubject(int subjectCode, int studentCode){
	char *data;
	char *sql = calloc(1024,sizeof(char));
	sprintf(sql,"SELECT CODIGO FROM INSCRIPCION NATURAL JOIN EXAMEN WHERE CODIGO = %d AND LEGAJO = %d AND NOTA >=4", subjectCode,studentCode);
	int num;
	int correlatives;

	
	data = query(sql);

	 num = atoi(data);

	if(num != 0){
		printf("Usted ya se inscribio en la materia.");
		return -1;
	}
	else{
		sprintf(sql, "SELECT CUPO FROM MATERIA WHERE CODIGO = %d", subjectCode);

		data = query(sql);
		num = atoi(data);


		if(num > 0){
			sprintf(sql, "SELECT COUNT(*) FROM (SELECT CORRELATIVA_DE FROM CORRELATIVA WHERE CODMATERIA = %d) AS AUXILIAR(MATERIAS_CORRELATIVAS) NATURAL JOIN EXAMEN WHERE NOTA >=4 AND CODIGO = %d AND LEGAJO= %d ", subjectCode, subjectCode,studentCode);
			data = query(sql);
			correlatives = atoi(data);

			sprintf(sql,"SELECT COUNT(*) FROM CORRELATIVA WHERE CODMATERIA = %d", subjectCode);
			data = query(sql);
			num = atoi(data);

			if(correlatives == num){
				sprintf(sql,"INSERT INTO INSCRIPCION(CODIGO, LEGAJO, ANIO) VALUES (%d,%d,SELECT date('now'))" , subjectCode, studentCode );
				data = query(sql);

			}else{
				printf("Usted no curso/aprobo las materias correlativas\n");
				return -1;
			}

		}
	}

	printf("%d\n",num );
	return 1;
	
}


int cancelSubscription(int subjectCode, int studentCode){
	char *data;
	char *sql = calloc(1024,sizeof(char));
	int num;

	sprintf(sql, "SELECT COUNT(*) FROM INSCRIPCION WHERE CODIGO = %d AND LEGAJO = %d", subjectCode,studentCode);
	data = query(sql);

	num = atoi(data);

	if(num != 0){
		printf("Usted no se encuentra inscripto en esta materia.\n");
		return -1;
	}else{
		sprintf(sql, "DELETE FROM INSCRIPCION WHERE CODIGO = %d AND LEGAJO = %d", subjectCode,studentCode);
		data = query(sql);
		printf("Usted ha cancelado la subscripcion a la materia de codigo: %d de forma exitosa.", subjectCode);
	}

	return 1;
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
	//tableCreation();
	sprintf(sql, "SELECT NOMBRE FROM MATERIA");
	data = query(sql);
	close();
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
	
	close();
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

	sprintf(sql, "SELECT  DISTINCT CARRERA FROM ALUMNO WHERE LEGAJO = %d", studentCode);
	carrera = query(sql);
	strncpy(carrera2, carrera, strlen(carrera)-2);

	sprintf(sql,"SELECT NOMBRE, COALESCE(NOTA, '-') FROM (SELECT CODIGO, NOMBRE FROM MATERIA WHERE CARRERA = '%s') AUXI LEFT OUTER JOIN (SELECT LEGAJO,CODIGO,NOTA FROM EXAMEN WHERE LEGAJO = %d AND NOTA >= 4) AUXI2 ON AUXI.CODIGO = AUXI2.CODIGO", carrera2,studentCode);
	data = query(sql);

	return data;

}

char * checkUser(int username, char * password){
	char * data;
	char *sql = calloc(1024,sizeof(char));
	//int legajo = atoi(username);
	int num;
	init();
	//tableCreation();

	printf("$$$$$$$$$$$$COMO USER NAME ME LLEGA:$$$$$$$$$$$$$$$$$$4\n");
	printf("$$$$$$$$$$$$%d$$$$$$$$$$$$$$$$$$$$$$",username);

	printf("\n\n\n&&&&&&&&&&COMO PASSWORD ME LLEGA: &&&&&&&&&&&\n");
	printf("&&&&&&&&&&%s&&&&&&&&&&&\n",password);

	printf("\n\nENTRE ENNNNNNN CHECKUSER\n");

	sprintf(sql, "SELECT COUNT(*) FROM ALUMNO WHERE LEGAJO = %d", username);
	data = query(sql);
	num = atoi(data);

	printf("Estoy por ingresar en el if Y NUM VALE: %d", num);

	if(num == 0){
		printf("Usuario incorrecto.\n");
		//close();
		return "USUARIO INCORRECTO";
	}else{
		if(checkPassword(username, password) == -1){
			printf("Contrasena incorrecta.\n");
			close();
			return "USUARIO INCORRECTO";
		}
	}
	printf("ESTOY POR SALIR\n");
	close();
	return "CORRECTO";
}

int checkPassword(int legajo, char* password){
	char * data;
	char *sql = calloc(1024,sizeof(char));
	int answer;

	//init();
	//tableCreation();

	sprintf(sql, "SELECT COUNT(*) FROM ALUMNO WHERE LEGAJO = %d AND PASSWORD = '%s'",legajo,password);
	data = query(sql);

	answer = atoi(data);

	if(answer == 0)
		return -1;

	//close();

	return 1;
}
