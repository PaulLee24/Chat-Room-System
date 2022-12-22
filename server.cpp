#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

#define MAX_CHAR 1026
#define LISTENQ 1000

struct ClientINFO
{
    string name;
    string ip;
    string port;
	time_t start;
    ClientINFO()
    {
        name = "--";
        ip = "--";
        port = -1;
		start = 0;
    }
};

bool cmd_operation(char *cmd_line, int client[], int i, int maxi, ClientINFO Info[])
{
    char *cmd = strtok(cmd_line, " \n");

    cout << "In cmd operation func: " << cmd << endl;

    if (cmd)
    {
        if (strcmp(cmd, "who") == 0)
        {
            cout << "Start who operation" << endl;
            string output;
            for (int j = 0; j <= maxi; j++)
            {
                if (client[j] > 2)
                {
                    time_t cur_time = time(NULL);
					int on = cur_time - Info[j].start;
					char time_buf[50];
                    sprintf(time_buf, "%d", on);
					output = "[Server] ";
                    output = output + Info[j].name + " " + Info[j].ip + ":" + Info[j].port + " online " + time_buf + "s";
                    if (j == i)
                    {
                        output = output + " ->me";
                    }
                    output = output + "\n";
                    char outstr[MAX_CHAR];
                    stpcpy(outstr, output.c_str());
                    cout << outstr << endl;
                    write(client[i], outstr, strlen(outstr));
                    //write(client[i], output.c_str(), output.size());
                }
            }
        }
        else if (strcmp(cmd, "name") == 0)
        {
            cmd = strtok(NULL, "\n");
            string cmd_s = cmd;
            int name_len = strlen(cmd);
            string output;
            if (strcmp(cmd, "anonymous") == 0)
            {
                output = "[Server] ERROR: Username cannot be anonymous.\n";
                char outstr[MAX_CHAR];
                stpcpy(outstr, output.c_str());
                cout << outstr << endl;
                write(client[i], outstr, strlen(outstr));
                //write(client[i], output.c_str(), output.size());
            }
            else if (name_len < 2 || name_len > 12)
            {
                output = "[Server] ERROR: Username can only consists of 2~12 English letters.\n";
                char outstr[MAX_CHAR];
                stpcpy(outstr, output.c_str());
                cout << outstr << endl;
                write(client[i], outstr, strlen(outstr));
                //write(client[i], output.c_str(), output.size());
            }
            else
            {
                // check uniqueness
                bool uniq = 1;
                for (int j = 0; j <= maxi; j++)
                {
                    if (client[j] > 2 && (Info[j].name == cmd_s) && (j != i))
                    {
                        output = "[Server] ERROR: ";
                        output = output + cmd_s + " has been used by others.\n";
                        char outstr[MAX_CHAR];
                        stpcpy(outstr, output.c_str());
                        cout << outstr << endl;
                        write(client[i], outstr, strlen(outstr));
                        //write(client[i], output.c_str(), output.size());
                        uniq = 0;
                        break;
                    }
                }
                //
                if (uniq)
                {
                    for (int j = 0; j <= maxi; j++)
                    {
                        if ((client[j] > 0) && (j != i))
                        {
                            output = "[Server] ";
                            output = output + Info[i].name + " is now known as " + cmd + ".\n";
                            char outstr[MAX_CHAR];
                            stpcpy(outstr, output.c_str());
                            cout << outstr << endl;
                            write(client[j], outstr, strlen(outstr));
                            //write(client[j], output.c_str(), output.size());
                        }
                    }
                    Info[i].name = cmd;
                    output = "[Server] You're now known as ";
                    output = output + Info[i].name + ".\n";
                    char outstr[MAX_CHAR];
                    stpcpy(outstr, output.c_str());
                    cout << outstr << endl;
                    write(client[i], outstr, strlen(outstr));
                    //write(client[i], output.c_str(), output.size());
                }
            }
        }
        else if (strcmp(cmd, "yell") == 0)
        {
            cmd = strtok(NULL, "\n");
            string output;
            for (int j = 0; j <= maxi; j++)
            {
                if (client[j] > 0)
                {
                    output = "[Server] ";
                    output = output + Info[i].name + " yell " + cmd + "\n";
                    char outstr[MAX_CHAR];
                    stpcpy(outstr, output.c_str());
                    cout << outstr << endl;
                    write(client[j], outstr, strlen(outstr));
                    //write(client[j], output.c_str(), output.size());
                }
            }
        }
        else if (strcmp(cmd, "tell") == 0)
        {
            cmd = strtok(NULL, " \n");
            string name = cmd;
            string output;
            char *message = strtok(NULL, "\n");
            bool error = 0;
            if (Info[i].name == "anonymous")
            {
                output = "[Server] ERROR: You are anonymous.\n";
                char outstr[MAX_CHAR];
                stpcpy(outstr, output.c_str());
                cout << outstr << endl;
                write(client[i], outstr, strlen(outstr));
                error = 1;
            }
            if (name == "anonymous")
            {
                output = "[Server] ERROR: The client to which you sent is anonymous.\n";
                char outstr[MAX_CHAR];
                stpcpy(outstr, output.c_str());
                cout << outstr << endl;
                write(client[i], outstr, strlen(outstr));
                error = 1;
            }
			else{
				bool match = 0;
				for (int j = 0; j <= maxi; j++)
				{
					if (client[j] > 0)
					{
						if (Info[j].name == name)
						{
							match = 1;
							if (!error)
							{
								output = "[Server] SUCCESS: Your message has been sent.\n";
								char outstr[MAX_CHAR];
								stpcpy(outstr, output.c_str());
								cout << outstr << endl;
								write(client[i], outstr, strlen(outstr));
								output = "[Server] ";
								output = output + Info[i].name + " tell you " + message + "\n";
								char outstr2[MAX_CHAR];
								stpcpy(outstr2, output.c_str());
								cout << outstr2 << endl;
								write(client[j], outstr2, strlen(outstr2));
							}
							break;
						}
					}
				}
				if (!match)
				{
					output = "[Server] ERROR: The receiver doesn't exist.\n";
					char outstr[MAX_CHAR];
					stpcpy(outstr, output.c_str());
					cout << outstr << endl;
					write(client[i], outstr, strlen(outstr));
				}
			}            
        }
        else if (strcmp(cmd, "exit") == 0)
        {
            return true;
        }
        else
        {
            string output;
            output = "[Server] ERROR: Error command.\n";
            char outstr[MAX_CHAR];
            stpcpy(outstr, output.c_str());
            cout << outstr << endl;
            write(client[i], outstr, strlen(outstr));
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        exit(1);
    }

    int i, maxi, max_fd, listen_fd, conn_fd, sockfd, n_ready;
    int client[FD_SETSIZE];
    ClientINFO Info[FD_SETSIZE];

    string output;
    char cmd_line[MAX_CHAR];
    ssize_t n;
    fd_set rset, allset;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    //cout << "Before bind" << endl;

    bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listen_fd, LISTENQ);

    max_fd = listen_fd + 1;
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++)
    {
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(listen_fd, &allset);

    //cout << "before while" << endl;

    while (1)
    {
        cout << "Enter new loop , maxfd = " << max_fd << endl;
        rset = allset;
        n_ready = select(max_fd, &rset, NULL, NULL, NULL);
        cout << "new listen" << endl;
        if (FD_ISSET(listen_fd, &rset))
        {
            clilen = sizeof(cliaddr);
            conn_fd = accept(listen_fd, (struct sockaddr *)&cliaddr, &clilen);

            for (i = 0; i < FD_SETSIZE; i++)
            {
                if (client[i] < 0)
                {
                    client[i] = conn_fd;
                    Info[i].name = "anonymous";
                    ///
                    Info[i].ip = inet_ntoa(cliaddr.sin_addr);
					Info[i].start = time(NULL);
                    unsigned short port_temp = ntohs(cliaddr.sin_port);
                    char port_buf[50];
                    sprintf(port_buf, "%hu", port_temp);
                    Info[i].port = port_buf;
                    ///
                    break;
                }
            }

            cout << "i: " << i << " connfd: " << conn_fd << endl;

            if (i == FD_SETSIZE)
            {
                cout << "Too many clients!" << endl;
                exit(2);
            }
            FD_SET(conn_fd, &allset);

            // Announce a new user is coming
            output = "[Server] Someone is coming!\n";
            cout << "New client!" << endl;
            for (int j = 0; j <= maxi; j++)
            {
                if ((client[j] > 0) && (j != i))
                {
                    char outstr[MAX_CHAR];
                    stpcpy(outstr, output.c_str());
                    cout << outstr << endl;
                    write(client[j], outstr, strlen(outstr));
                    //write(client[j], output.c_str(), output.size());
                }
            }
            //

            // Print hello message
            output = "[Server] Hello, anonymous! From: ";
            output = output + Info[i].ip + ":" + Info[i].port + "\n";
            char outstr[MAX_CHAR];
            stpcpy(outstr, output.c_str());
            cout << outstr << endl;
            //cout << strlen(outstr) << endl;
            write(client[i], outstr, strlen(outstr));
            //write(client[i], output.c_str(), output.size());
            //

            if (conn_fd >= max_fd)
            {
                max_fd = conn_fd + 1;
            }
            if (i > maxi)
            {
                maxi = i;
            }
            n_ready--;
            if (n_ready <= 0)
            {
                continue;
            }
        }

        cout << "After deal with listen" << endl;

        for (i = 0; i <= maxi; i++)
        {
            if (client[i] < 0)
            {
                continue;
            }
            if (FD_ISSET(client[i], &rset))
            {
                //cout << "Before read" << endl;
                n = read(client[i], cmd_line, MAX_CHAR);
                cmd_line[n] = '\0';
                //cout << "After readline" << endl;
                if (n == 0 || cmd_operation(cmd_line, client, i, maxi, Info))
                {
                    cout << "Close client " << i << endl;
                    output = "[Server] ";
                    output = output + Info[i].name + " is offline.\n";
                    for (int j = 0; j <= maxi; j++)
                    {
                        if ((client[j] > 0) && (j != i))
                        {
                            char outstr[MAX_CHAR];
                            stpcpy(outstr, output.c_str());
                            cout << outstr << endl;
                            write(client[j], outstr, strlen(outstr));
                            //write(client[j], output.c_str(), output.size());
                        }
                    }
                    close(client[i]);
                    FD_CLR(client[i], &allset);
                    client[i] = -1;
                }
                cout << "After cmd operation" << endl;
                n_ready--;
                if (n_ready <= 0)
                {
                    break;
                }
            }
        }
    }

    return 0;
}
