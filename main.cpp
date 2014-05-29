#import <cstdio>
#import <cstdlib>
#import <vector>
#import <iostream>


enum state {idle, set, 
			setN = 'N', setK = 'K', setH = 'H', setZ = 'Z',
			setE = 'E'};

#define SET 370

using namespace std;
			
int _hash(char*);

int main(int argc, const char * argv[])
{	
	FILE *file = fopen(argv[1], "r");

	int N = 0, K = 0, H = 0, Z = 0;
	vector < pair <int, int> > E;
	
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
		
				if(buffer[bufferCount - 1] == EOF) break;
				buffer[bufferCount - 1] = '\0';
				switch (_hash(buffer))
				{
					case SET:
					state = set;
				}
				break;
			case set:
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
						printf("\nH is %i\n", H);
						break;
					case setZ:
						Z = value;
						printf("\nZ is %i\n", H);
						return 0;
						break;
				}
				state = idle;
				break;

			case setE:
				printf("E");
				int values[2] = {0, 0};
				int tmp = 0;

				for(int i=0 ; i<2 ; i++) fgetc(file);	// :=
				while(1)
				{
					bufferCount = 0;
					do buffer[bufferCount++] = fgetc(file);
					while (	buffer[bufferCount - 1] != '(' && 
							buffer[bufferCount - 1] != ')' &&
							buffer[bufferCount - 1] != '\n'&&
							buffer[bufferCount - 1] != ','&&
							buffer[bufferCount - 1] != ';');
					
					if (buffer[bufferCount - 1] ==  ';') break;

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
					
				}
				state = idle;
				break;
				
			
		}
		//printf("|%s|%i|\n", buffer, hash(buffer));
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