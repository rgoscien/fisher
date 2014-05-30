#import <cstdio>
#import <cstdlib>
#import <vector>
#import <iostream>

struct demand
{
	int src, dst, volume;
};

enum state {idle = 0, set = 370, param = 1391, 
			setN = 'N', setK = 'K', setH = 'H', setZ = 'Z',
			setE = 'E',
			paramF = 'F', paramG = 'G', paramC = 'C',
			paramS = 'S', paramT = 'T', paramL = 'L',
			paramR = 'R', paramA = 'A'};

using namespace std;

int _hash(char*);

int main(int argc, const char * argv[])
{
	FILE *file = fopen(argv[1], "r");

	int N = 0, K = 0, H = 0, Z = 0, S=0, ***L, *****R, ******A;
	vector < pair <int, int> > E;
	vector <demand> T;
	float *F, *C;
	double G;
	
	char buffer[256];
	int bufferCount = 0, value = 0;
	
	state state = idle;
	
	while (file)
	{
		bufferCount = 0;
		
		// Get word
		switch (state)
		{		
			case idle:
				do buffer[bufferCount++] = fgetc(file);
				while (	buffer[bufferCount - 1] != ' ' && 
						buffer[bufferCount - 1] != '\n' && 
						buffer[bufferCount - 1] != EOF );		
		
				if(buffer[bufferCount - 1] == EOF) return 0;
				buffer[bufferCount - 1] = '\0';
				//printf("\nIdle: %s [%i]\n\n", buffer, _hash(buffer));
				switch (_hash(buffer))
				{
					case set:
						state = set;
						break;

					case param:
						state = param;
						break;

					default:
						break;
				}
				break;
			case set:
				state = (enum state)fgetc(file);
				break;
			case param:
				state = (enum state)fgetc(file);
				break;
			case setN:
			case setK:
			case setH:
			case setZ:
				for(int i=0 ; i<2 ; i++) fgetc(file);	// :=
				while(1)
				{
					bufferCount = 0;
					do buffer[bufferCount++] = fgetc(file);
					while (	buffer[bufferCount - 1] != ' ' && 
							buffer[bufferCount - 1] != '\t' &&
							buffer[bufferCount - 1] != '\n' && 
							buffer[bufferCount - 1] != ';');
					
					if (buffer[bufferCount - 1] ==  '\n') break;

					buffer[bufferCount - 1] = '\0';
					//printf("\t|%s|\n", buffer);
					value = atoi(buffer) != 0 ? atoi(buffer) : value;
				}
				
				switch (state)
				{
					case setN:
						N = value;
						printf("\nN is %i\n", N);
						break;
					case setK:
						K = value;
						printf("\nK is %i\n", K);
						break;
					case setH:
						H = value;
						F = new float[H];
						C = new float[H];
						printf("\nH is %i\n", H);
						break;
					case setZ:
						Z = value;
						printf("\nZ is %i\n", Z);
						break;
					default:
						break;
				}
				state = idle;
				break;

			case setE:
			{
				printf("E");
				int values[2] = {0, 0};
				int tmp = 0;

				for(int i=0 ; i<2 ; i++) fgetc(file);	// :=
				
				bool breakNext = false;
				while(1)
				{
					bufferCount = 0;
					do buffer[bufferCount++] = fgetc(file);
					while (	buffer[bufferCount - 1] != '(' && 
							buffer[bufferCount - 1] != ')' &&
							buffer[bufferCount - 1] != '\n'&&
							buffer[bufferCount - 1] != ','&&
							buffer[bufferCount - 1] != ';');
					
					if (buffer[bufferCount - 1] ==  ';') breakNext = true;

					buffer[bufferCount - 1] = '\0';
					value = atoi(buffer) != 0 ? atoi(buffer) : -1;
				
					if (value != -1)
					{
						values[tmp % 2] = value;
						if (tmp++ % 2 == 1)
						{
							printf("\n\tadding pair %i %i", values[0], values[1]);
							E.push_back(make_pair(values[0], values[1] ));
						}
					}
					if (breakNext) break;	
				}
				state = idle;
				break;
			}
			case paramF:
			case paramC:
			{
				printf("F or C");
				float value = 0;
				int tmp = 0, tmp2 = 0;

				for(int i=0 ; i<2 ; i++) fgetc(file);	// :=
				
				bool breakNext = false;
				while(1)
				{
					bufferCount = 0;
					do buffer[bufferCount++] = fgetc(file);
					while (	buffer[bufferCount - 1] != '\t' &&
							buffer[bufferCount - 1] != '\n'&&
							buffer[bufferCount - 1] != ';');
					
					if (buffer[bufferCount - 1] ==  ';') breakNext = true;
					
					buffer[bufferCount - 1] = '\0';
					value = atof(buffer) != 0 ? atof(buffer) : -1;
				
					if (value != -1 && tmp++ % 2 == 1)
					{
						switch(state)
						{
							case paramF:
								F[tmp2++] = value;
								break;
							case paramC:
								C[tmp2++] = value;
								break;
							default:
								break;
						}
						printf("\n\thaving value %.2f", value);
					}
					if (breakNext) break;
				}
				
				state = idle;
				break;
			}
			case paramG:
				for(int i=0 ; i<2 ; i++) fgetc(file);		// :=
				bufferCount = 0;
				do buffer[bufferCount++] = fgetc(file);
				while (buffer[bufferCount - 1] != ';');
				
				G = atof(buffer);
				printf("\nG is %f\n", G);
				
				state = idle;
				break;

			case paramS:
				for(int i=0 ; i<2 ; i++) fgetc(file);		// :=
				bufferCount = 0;
				do buffer[bufferCount++] = fgetc(file);
				while (buffer[bufferCount - 1] != ';');
			
				S = atoi(buffer);
				printf("\nS is %i\n", S);
			
				state = idle;
				break;
			case paramT:
			{
				printf("\nT\n");
				
				do buffer[0] = fgetc(file);					// Skip line
				while (buffer[0] != '\n');
				
				int tmp = 0, src = 0, dst = 0;
				while(1)
				{
					bufferCount = 0;
					do buffer[bufferCount++] = fgetc(file);
					while (	buffer[bufferCount - 1] != ' ' && 
							buffer[bufferCount - 1] != '\t' &&
							buffer[bufferCount - 1] != '\n' &&
							buffer[bufferCount - 1] != ';');
					
					if (buffer[bufferCount - 1] ==  ';') break;
					if (bufferCount==1) continue;
					
					buffer[bufferCount - 1] = '\0';
					
					dst = tmp % (N + 2);
					
					if(!dst) src = atoi(buffer);			// Index 0
					else if (dst < N)						// Workaround
					{
						demand demand = (struct demand){src, dst, atoi(buffer)};
						if(demand.volume)
						{
							T.push_back(demand);
							printf("\t%i -> %i : %i\n", demand.src, demand.dst, demand.volume);
						}
					}
					tmp++;
				}
				printf("%lu elements pushed\n", T.size());
				state = idle;
				break;
			}
			case paramL:
			{
				printf("L [%i:%i:%i]", N, N, K);
				L = new int**[N];
				for(int i=0 ; i<N ; i++)
				{
					L[i] = new int*[N];
					for (int j=0 ; j<N ; j++)
						L[i][j] = new int[K];
				}
				
				for(int i=0 ; i<3 ; i++) fgetc(file);		// :=[
				
				while(true)
				{
					// Odczytywanie adresu tablicy [*,*,2]
					bool willBreak = false;
					int k = 0;
					while(1)
					{
						bufferCount = 0;
						do buffer[bufferCount++] = fgetc(file);
						while (	buffer[bufferCount - 1] != ',' &&
								buffer[bufferCount - 1] != ']' &&
								buffer[bufferCount - 1] != ':');
					
						if (buffer[bufferCount - 1] ==  ':') willBreak = true;
					
						if (buffer[bufferCount - 2] !=  '*' && bufferCount > 1)
						{
							buffer[bufferCount - 1] = '\0';
							k = atoi(buffer);
							printf("\nvalue %i", k);
						}
					
						buffer[bufferCount - 1] = '\0';
					
						printf("\n[%s]", buffer);
					
						if (willBreak) break;
					}
				
					printf("\n\n*:*:%i\n\n", k);
				
					bool lastTable = false;
					// Odczytywanie tablicy
					int tmp = 0, i = 0, j = 0, value = 0;
					while(1)
					{
						bufferCount = 0;
						do buffer[bufferCount++] = fgetc(file);
						while (	buffer[bufferCount - 1] != ' ' && 
								buffer[bufferCount - 1] != '\t' &&
								buffer[bufferCount - 1] != '\n' &&
								buffer[bufferCount - 1] != ';' &&
								buffer[bufferCount - 1] != '[');
					
						if (buffer[bufferCount - 1] ==  '[') break;
						if (buffer[bufferCount - 1] ==  ';')
						{
							lastTable = true;
							break;
						}
						if (bufferCount==1) continue;
					
						buffer[bufferCount - 1] = '\0';
					
						i = tmp % (N + 2) - 1;
						j = (tmp - 1) / (N + 2);
					
						if (tmp > N + 1 && i > 0)
						{
							value = atoi(buffer);
							printf("%i:%i:%i = %i\n",i-1, j-1, k-1, value);
							L[i-1][j-1][k-1] = value;
						}
					
						tmp++;
					}
					if(lastTable) break;
				}
				state = idle;
				break;
			}

			case paramR:
			{
				printf("R [%i:%i:%i:%i:%i]", N, N, K, H, Z);
								
				R = new int****[N];
				for(int i=0 ; i<N ; i++)
				{
					R[i] = new int***[N];
					for (int j=0 ; j<N ; j++)
					{
							R[i][j] = new int**[K];
							for (int k=0 ; k<K ; k++)
							{
								R[i][j][k] = new int*[H];
								for (int h=0 ; h<H ; h++)
									R[i][j][k][h] = new int[Z];
							}
					}
				}
				
				for(int i=0 ; i<3 ; i++) fgetc(file);		// :=[
				
				while(true)
				{
					// Odczytywanie adresu tablicy [*,*,2]
					bool willBreak = false;
					int khz[3] = {0, 0, 0};
					int khzCount = 0;
					
					while(1)
					{
						bufferCount = 0;
						do buffer[bufferCount++] = fgetc(file);
						while (	buffer[bufferCount - 1] != ',' &&
								buffer[bufferCount - 1] != ']' &&
								buffer[bufferCount - 1] != ':');
					
						if (buffer[bufferCount - 1] ==  ':') willBreak = true;
						
						if (buffer[bufferCount - 2] !=  '*' && bufferCount > 1)
						{
							buffer[bufferCount - 1] = '\0';
							khz[khzCount++] = atoi(buffer);
							printf("\n%i value is %i",khzCount-1, khz[khzCount-1]);
						}
					
						buffer[bufferCount - 1] = '\0';
					
						printf("\n[%s]", buffer);
					
						if (willBreak) break;
					}
					
					khzCount = 0;
					printf("\n\n*:*:%i:%i:%i\n\n", khz[0], khz[1], khz[2]);
					//break;
					
					bool lastTable = false;
					// Odczytywanie tablicy
					int tmp = 0, i = 0, j = 0, 
						k = khz[0], h = khz[1], z = khz[2], 
						value = 0;
						
					
					while(1)
					{
						bufferCount = 0;
						do buffer[bufferCount++] = fgetc(file);
						while (	buffer[bufferCount - 1] != ' ' && 
								buffer[bufferCount - 1] != '\t' &&
								buffer[bufferCount - 1] != '\n' &&
								buffer[bufferCount - 1] != ';' &&
								buffer[bufferCount - 1] != '[');
				
						if (buffer[bufferCount - 1] ==  '[') break;
						if (buffer[bufferCount - 1] ==  ';')
						{
							lastTable = true;
							break;
						}
						if (bufferCount==1) continue;
				
						buffer[bufferCount - 1] = '\0';
				
						i = tmp % (N + 2) - 1;
						j = (tmp - 1) / (N + 2);
				
						if (tmp > N + 1 && i > 0)
						{
							value = atoi(buffer);
							printf("%i:%i:%i:%i:%i = %i\n",i, j, k, h, z, value);
							R[i-1][j-1][k-1][h-1][z-1] = value;
						}
				
						tmp++;
					}
					if(lastTable) break;
				}
				state = idle;
				break;
			}
			case paramA:
				printf("\nA [%i:%i:%i:%i:%i,%i]", N, N, K, N, N, K);
				
				A = new int*****[N];
				for(int i=0 ; i<N ; i++)
				{
					A[i] = new int****[N];
					for (int j=0 ; j<N ; j++)
					{
							A[i][j] = new int***[K];
							for (int k=0 ; k<K ; k++)
							{
								A[i][j][k] = new int**[N];
								for (int i_=0 ; i_<N ; i_++)
								{
									A[i][j][k][i_] = new int*[N];
									for (int j_=0 ; j_<N ; j_++)
										A[i][j][k][i_][j_] = new int[K];
								}
							}
					}
				}
				
				
				for(int i=0 ; i<3 ; i++) fgetc(file);		// :=[
				while(true)
				{
					// Odczytywanie adresu tablicy [*,*,2]
					bool willBreak = false;
					int kijk[4] = {0, 0, 0, 0};
					int kijkCount = 0;
					
					while(1)
					{
						bufferCount = 0;
						do buffer[bufferCount++] = fgetc(file);
						while (	buffer[bufferCount - 1] != ',' &&
								buffer[bufferCount - 1] != ']' &&
								buffer[bufferCount - 1] != ':');
					
						if (buffer[bufferCount - 1] ==  ':') willBreak = true;
						
						if (buffer[bufferCount - 2] !=  '*' && bufferCount > 1)
						{
							buffer[bufferCount - 1] = '\0';
							kijk[kijkCount++] = atoi(buffer);
							printf("\n%i value is %i",kijkCount-1, kijk[kijkCount-1]);
						}
					
						buffer[bufferCount - 1] = '\0';
					
						printf("\n[%s]", buffer);
					
						if (willBreak) break;
					}
					
					kijkCount = 0;
					printf("\n\n*:*:%i:%i:%i:%i\n\n", kijk[0], kijk[1], kijk[2], kijk[3]);
					
					bool lastTable = false;
					// Odczytywanie tablicy
					int tmp = 0, i = 0, j = 0, 
						k = kijk[0], i_ = kijk[1], j_ = kijk[2], k_ = kijk[3], 
						value = 0;
						
					
					while(1)
					{
						bufferCount = 0;
						do buffer[bufferCount++] = fgetc(file);
						while (	buffer[bufferCount - 1] != ' ' && 
								buffer[bufferCount - 1] != '\t' &&
								buffer[bufferCount - 1] != '\n' &&
								buffer[bufferCount - 1] != ';' &&
								buffer[bufferCount - 1] != '[');
				
						if (buffer[bufferCount - 1] ==  '[') break;
						if (buffer[bufferCount - 1] ==  ';')
						{
							lastTable = true;
							break;
						}
						if (bufferCount==1) continue;
				
						buffer[bufferCount - 1] = '\0';
				
						i = tmp % (N + 2) - 1;
						j = (tmp - 1) / (N + 2);
				
						if (tmp > N + 1 && i > 0)
						{
							value = atoi(buffer);
							printf("%i:%i:%i:%i:%i:%i = %i\n",i, j, k, i_, j_, k_, value);
							A[i-1][j-1][k-1][i_-1][j_-1][k_-1] = value;
						}
				
						tmp++;
					}
					if(lastTable) break;
				}
				state = idle;
				break;
		}
	}
	fclose(file);
}

int _hash(char* string)
{
	int _hash = 0, i = 0;
	do _hash = _hash << 1 ^ string[i++];
	while (string[i] != '\0');
	return _hash;
}