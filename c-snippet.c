/*https://cboard.cprogramming.com/cplusplus-programming/147455-how-receive-large-data-using-recv-function-cplusplus.html*/
char c;
               char temp[1024*1024];
               while (!found) {
                   n = recv(GetSocketId(), &temp[total], sizeof(temp) - total - 1, 0);
                   if (n == -1) {
                        fprintf(stderr, "Error recv data %d\n", errno);
                        return LS_RESULT_FAILED;

                   }
                   total += n;
                   temp[total] = '\0';
                   found = (strchr(temp, '!') != 0);
               }
