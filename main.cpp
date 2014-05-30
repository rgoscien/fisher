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

	int N = 0, K = 0, H = 0, Z = 0, S=0;
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
				
				do buffer[0] = fgetc(file);
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
							printf("\t%i -> %i : %i\n", demand.src,
														demand.dst, 
														demand.volume);
						}
					}
					tmp++;
				}
				printf("%lu elements pushed\n", T.size());
				state = idle;
				break;
			}
			case paramA:
			case paramL:
			case paramR:
				printf("\nskip\n");
				state = idle;
				break;
				//return 0;
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