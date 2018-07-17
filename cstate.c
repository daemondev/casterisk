#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

int main(int argc, char *argv[]) {
    char *str = "update agent_state set state='TALKING' where callerid=%s and day=now()::date";
    char sql[100];
    snprintf(sql, 100, str, argv[1]);

    PGconn *conn = PQconnectdb("host=ryr.homeplex.org port=55443 user=asterisk dbname=asterisk password=$asterisk$123$");
    /*PGconn *conn = PQconnectdb("host=172.16.16.9 port=5432 user=asterisk dbname=asterisk password=$asterisk$123$");*/
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stdout, "EXEC NOOP Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn); exit(1);
    }

    PGresult *res = PQexec(conn, sql);
    //PGresult *res = PQexecParams(conn, sql, 1, NULL,  paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);PQfinish(conn); exit(1);
    }
    PQclear(res);
    PQfinish(conn);
}

/*gcc cstate.c -lws2_32 -I"C:\Program Files\PostgreSQL\9.4\include" -L"C:\Program Files\PostgreSQL\9.4\lib" -lpq -o cstate*/
/*https://www.postgresql.org/ftp/binary/v8.2.11/*/
