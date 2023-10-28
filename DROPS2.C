//
//
// ********************************************************************
// **********                                                **********
// ********** DROPS 2 - ZICAU SOFTWARE LIMITADA - ITAJAI, SC **********
// **********                                                **********
// ********************************************************************
//
//   Esta versao e' apenas uma BETA (BETA-1). Para maiores informacoes,
//
// contate a 'ZiCau Soft'  -  Fone: ------------- (falar com jacknpoe).
//
//
// OBS. 20/OUT/2013 > ESTA VERSÃO E' DE 1994 E NO MOMENTO:
// -NÃO IRÁ COMPILAR A NÃO SER SE A BIBLIOTECA conio.h E ALGUMAS OPÇÕES
// ESTIVEREM MARCADAS P/ COMPATIBILIZAÇÃO COM OS COMPILADORES MODERNOS
// -SE COMPILAR, NÃO IRÁ RODAR NOS SISTEMAS OPERACIONAIS ATUAIS,  SENDO
// NECESSÁRIO WINDOWS XP OU ANTERIOR
// -SE RODAR, PRECISARÁ DO ARQUIVO drops2.dat COM AS IMAGENS DO JOGO
//


#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

#define S_IREAD 0x0100
#define S_IWRITE 0x0080

union REGS cau;

struct peca
{
	char primeira;
	char segunda;
	char terceira;
} peca1, peca_prox;

struct game
{
	char vidas;
	char fase;
	int  pecas;
	long escore;
	long melhor;
	char proximo;
	int  passo;
} jogo;

struct hi_score
{
	char n_jogador[ 11];
	char dificuldade;
	long n_pontos;
} melhores[ 10];

int acum;

char troquei_ints = 0;
char sem_placa = 0;
char erro_arq = 0;
char sem_memo = 0;
char deixou_tempo = 0;

int far *end_teclado = (int far *) 0x417;

char estado_som = 0;
char estado_antigo;
int  contador_efeito, tam_efeito, contador_musica, tam_musica;
int far *pt_efeito, *pt_musica;

char opt_cza = 1, opt_som = 0, opt_eft = 0, opt_cfg = 0;

char x_peca, y_peca, estilo = 0;

char menu_opt = 0, m_o_opt = 0, dif_opt = 1, vid_opt = 3, pec_opt = 0, ant_pec_opt = 0;

char ftemp[ 10][ 20];
char fosso[ 12][ 21];
char c_cfg[ 3];
char far *alfa=NULL, *zicau=NULL, *simp=NULL, *alfa12=NULL, *dropsii=NULL;
char mensagem[] = "APRESENTA";
char gameover[] = "FIM DE JOGO";
char highmess[] = "GALERIA DOS RECORDES";
char versaobeta[] = "VERS~AO BETA-1";
char nronovo[]  = "      ";
char editar[]   = "          ";
unsigned char m_o_x[] = { 94, 130,  40 };
unsigned char m_o_y[] = { 32, 112, 204 };
unsigned char m_o_xy[]= { 76, 104, 144, 196 };
unsigned char m_cfg[] = { 82, 124, 118 };
int  efect[] = { 440, 880, 440, 880, 1760};
char *nomes[] = { "BONHAM....","BURTON....","COBAIN....","HENDRIX...","JOPLIN....",
				  "LENNON....","MARLEY....","MORRISON..","PRESLEY...","SEIXAS...." };
char b_dificuldade[] = { 2, 2, 2, 1, 1, 1, 0, 0, 0, 0};
long b_pontos[] = { 100000, 90000, 80000, 70000, 60000,
				  50000, 40000, 30000, 20000, 10000};
char *digite[] = { "VOC^E ENTROU NA", "DIGITE SEU NOME:" };
char *menu[] = { "COME€A", "OP€~OES", "CONFIG", "D.O.S." };
char *config[] = { "MONOCROM@ATICO", "M@USICA", "EFEITOS" };
char *simnao[] = { "SIM", "N~AO" };
char *opcoes[] = { "DIFICULDADE", "VIDAS", "PE€AS",
				   "F@ACIL", "NORMAL", "DIF@ICIL",
				   "l", "ll", "lll", "llll" };
char *escore[] = { "VIDAS", "ESCORE", "MELHOR", "FASE", "PE€AS", "PR@OX." };
char *staff[] = { "INSPIRADO EM ORIGINAL DE", "ALEXEY PAJITNOV", "",
				  "PROJETADO POR", "RICARDO ERICK REB^ELO", "",
				  "PROGRAMADO POR", "RICARDO ERICK REB^ELO", "",
				  "M@USICAS E EFEITOS DE", "JOMAR MILIOLI GOUDINHO", "",
				  "GR@AFICOS POR", "RICARDO ERICK REB^ELO", "JOMAR MILIOLI GOUDINHO",
				  "PRODUZIDO NA", "ZICAU SOFTWARE LTDA.", "" };


// ***********************************************************
// ***************                             ***************
// *************** Rotinas e variaveis de INTs ***************
// ***************                             ***************
// ***********************************************************


unsigned int  estado_tecla[4];
unsigned int  tempo_tecla[4];
unsigned int  press_tecla[4];
unsigned int  hora_real = 0;
unsigned char auxiliar;
unsigned char scancode;

void interrupt (*velha_int_09h)();
void interrupt (*velha_int_1ch)();
void interrupt (*velha_int_1bh)();

void interrupt nova_int_1bh()
{
}

void interrupt nova_int_09h()
{
	scancode = inportb( 0x60);
	switch (scancode)
	{
		case  72: {	if( estado_tecla[0] == 0) press_tecla[0] = 1;
					estado_tecla[0] = 1; }
					break;
		case 200: {	estado_tecla[0] = 0;
					tempo_tecla[0] = 0; }
					break;
		case  77: {	if( estado_tecla[1] == 0) press_tecla[1] = 1;
					estado_tecla[1] = 1; }
					break;
		case 205: {	estado_tecla[1] = 0;
					tempo_tecla[1] = 0; }
					break;
		case  80: {	if( estado_tecla[2] == 0) press_tecla[2] = 1;
					estado_tecla[2] = 1; }
					break;
		case 208: {	estado_tecla[2] = 0;
					tempo_tecla[2] = 0; }
					break;
		case  75: {	if( estado_tecla[3] == 0) press_tecla[3] = 1;
					estado_tecla[3] = 1; }
					break;
		case 203: {	estado_tecla[3] = 0;
					tempo_tecla[3] = 0; }
					break;
	}
	if( ( ( ( scancode > 70) & ( scancode < 82)) | ( ( scancode > 198) & ( scancode < 210))) | scancode == 225)
	{
		auxiliar = inportb( 0x61);
		outportb( 0x61, auxiliar | 0x80);
		outportb( 0x61, auxiliar);
		outportb( 0x20, 0x20);
	}
	else (*velha_int_09h)();
}

void interrupt nova_int_1ch()
{
	hora_real++;
	if( estado_som == 0)
		nosound();
	else
	{
		if( estado_som == 2)
		{
			if( pt_efeito[ contador_efeito] > 0)
				sound( pt_efeito[ contador_efeito]);
			else
				nosound();
			contador_efeito++;
			if( contador_efeito == tam_efeito)
				estado_som = estado_antigo;
		}
		else
			if( pt_musica[ contador_musica] > 0)
				sound( pt_musica[ contador_musica]);
			else
				nosound();
		contador_musica++;
		if( contador_musica == tam_musica)
			contador_musica = 0;
	}

	if( estado_tecla[0] == 1)
	{
		tempo_tecla[0] += 1;
		if( tempo_tecla[0] > 11)
		{
			tempo_tecla[0] -= 12;
			press_tecla[0] = 1;
		}
	}
	if( estado_tecla[1] == 1)
	{
		tempo_tecla[1] += 1;
		if( tempo_tecla[1] > 8)
		{
			tempo_tecla[1] -= 9;
			press_tecla[1] = 1;
		}
	}
	if( estado_tecla[2] == 1)
	{
		tempo_tecla[2] += 1;
		if( tempo_tecla[2] > 11)
		{
			tempo_tecla[2] -= 12;
			press_tecla[2] = 1;
		}
	}
	if( estado_tecla[3] == 1)
	{
		tempo_tecla[3] += 1;
		if( tempo_tecla[3] > 8)
		{
			tempo_tecla[3] -= 9;
			press_tecla[3] = 1;
		}
	}

	(*velha_int_1ch)();
}


// ***********************************************************
// ***************                             ***************
// *************** Biblioteca Grafica para VGA ***************
// ***************                             ***************
// ***********************************************************


unsigned char far *vga_ini = (unsigned char far *) 0xa0000000L;

unsigned char asc_to_alfa[ 60] = { 32, 65, 66, 67, 68, 69, 70, 71, 72, 73,
								   74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
								   84, 85, 86, 87, 88, 89, 90,128, 45, 48,
								   49, 50, 51, 52, 53, 54, 55, 56, 57, 46,
								   44, 58, 33, 63, 64, 94,126, 96, 59, 34,
								   39, 40, 41, 61, 36, 47, 38,112,120,108 };

char detecta_vga( void)
{
	cau.x.ax = 0x1a00;
	int86 ( 0x10, &cau, &cau );
	if( cau.h.al != 0x1a ) return ( 0 );
	if( ( cau.h.bl == 0x08 ) || ( cau.h.bl == 0x0c ) ) return ( 1 );
	return ( 2 );
}

void cor( unsigned char cor1, char cor2, char cor3, char cor4)
{
	cau.x.ax= 0x1010;
	cau.h.dh= cor2;
	cau.h.ch= cor3;
	cau.h.cl= cor4;
	cau.x.bx= cor1;
	int86(0x10,&cau,&cau);
}

void vga( void)
{
	cau.h.ah= 0;
	cau.h.al= 0x13;
	int86(0x10,&cau,&cau);
}

void sobe( char y)
{
	cau.h.ah = 6;
	cau.h.al = 1;
	cau.h.bh = 0;
	cau.h.ch = y;
	cau.h.cl = 16;
	cau.h.dh = y + 4;
	cau.h.dl = 23;
	int86(0x10,&cau,&cau);
}

void desce( char y)
{
	cau.h.ah = 7;
	cau.h.al = 1;
	cau.h.bh = 0;
	cau.h.ch = y;
	cau.h.cl = 15;
	cau.h.dh = y + 1;
	cau.h.dl = 24;
	int86(0x10,&cau,&cau);
}

void closevga(void)
{
	cau.h.ah= 0;
	cau.h.al= 0x3;
	int86(0x10,&cau,&cau);
}

void barra(int x, int y, int tx, int ty, unsigned char color)
{
	register unsigned passox = 0, passoy = 0;
	unsigned inicial, passo;
	inicial = x + y * 320;
	passo = 320 - tx;
	while(passoy < ty)
	{
		while(passox < tx)
		{
			*(vga_ini + inicial) = color;
			inicial++;
			passox++;
		}
		passox = 0;
		inicial += passo;
		passoy++;
	}
}

void bitmap( int x, int y, unsigned char far *pt_bitmap)
{
	register unsigned char passox = 0, passoy = 0;
	register unsigned inicial;
	inicial = x + y * 320;
	while(passoy < 8)
	{
		while(passox < 8)
		{
			*(vga_ini + inicial) = *pt_bitmap;
			inicial++;
			pt_bitmap++;
			passox++;
		}
		passox = 0;
		inicial += 312;
		passoy++;
	}
}

void m_bitmap( int x, int y, unsigned char far *pt_bitmap)
{
	register unsigned char passox = 0, passoy = 0;
	register unsigned inicial;
	inicial = x + y * 320;
	while(passoy < 8)
	{
		while(passox < 8)
		{
			if(!(*pt_bitmap == 0)) *(vga_ini + inicial) = *pt_bitmap;
			inicial++;
			pt_bitmap++;
			passox++;
		}
		passox = 0;
		inicial += 312;
		passoy++;
	}
}

void m_bitmapc( int x, int y, unsigned char far *pt_bitmap, unsigned char cor)
{
	register unsigned char passox = 0, passoy = 0;
	register unsigned inicial;
	inicial = x + y * 320;
	cor-=1;
	while(passoy < 8)
	{
		while(passox < 8)
		{
			if(!(*pt_bitmap == 0)) *(vga_ini + inicial) = cor - passoy / 2;
			inicial++;
			pt_bitmap++;
			passox++;
		}
		passox = 0;
		inicial += 312;
		passoy++;
	}
}

void m_bitmap12( int x, int y, unsigned char far *pt_bitmap)
{
	register unsigned char passox = 0, passoy = 0;
	register unsigned inicial;
	inicial = x + y * 320;
	while(passoy < 12)
	{
		while(passox < 12)
		{
			if(!(*pt_bitmap == 0)) *(vga_ini + inicial) = *pt_bitmap;
			inicial++;
			pt_bitmap++;
			passox++;
		}
		passox = 0;
		inicial += 308;
		passoy++;
	}
}

void m_bitmap12c( int x, int y, unsigned char far *pt_bitmap, unsigned char cor)
{
	register unsigned char passox = 0, passoy = 0;
	register unsigned inicial;
	inicial = x + y * 320;
	while(passoy < 12)
	{
		while(passox < 12)
		{
			if(!(*pt_bitmap == 0)) *(vga_ini + inicial) = cor - passoy / 2;
			inicial++;
			pt_bitmap++;
			passox++;
		}
		passox = 0;
		inicial += 308;
		passoy++;
	}
}

void palheta_cor(void)
{
	register char a;
	char forte, fraco, correcao;
	for(a=1;a<13;a++)
	{
		forte = (double) a * 5.25 + .5;
		fraco = (double) a * 2.625 + .5;
		correcao = (double) a * .984375 + .5;
		cor(a+ 63,forte,0,0);
		cor(a+ 75,0,forte,0);
		cor(a+ 87,correcao,correcao,forte);
		cor(a+ 99,forte,forte,0);
		cor(a+111,forte,0,forte);
		cor(a+123,0,forte,forte);
		cor(a+135,forte,fraco,0);
		cor(a+147,fraco,forte,0);
		cor(a+159,forte,forte,fraco);
		cor(a+171,forte,0,fraco);
		cor(a+183,forte,fraco,forte);
		cor(a+195,fraco,0,forte);
		cor(a+207,fraco,forte,forte);
		cor(a+219,0,forte,fraco);
		cor(a+231,0,fraco,forte);
		cor(a+243,0,0,0);
	}
}

void palheta(void)
{
	register char a;
	for(a=0;a<64;a++) cor(a,a,a,a);
	palheta_cor();
}

void gray_scale( void)
{
	cau.x.ax = 0x101b;
	cau.x.bx = 64;
	cau.x.cx = 180;
	int86(0x10,&cau,&cau);
}

char transf(unsigned char caracter)
{
	register unsigned char passo = -1;
	while(!((passo++)==59))
	{
		if( caracter == asc_to_alfa[ passo]) return ( passo + 1);
	}
	return( 1);
}

void converte(unsigned char *texto)
{
	register unsigned char tam, passo = -1;
	tam = strlen( texto);
	tam--;
	while(!((passo++)==tam)) texto[ passo] = transf( texto [ passo]);
}

int put_text( int x, unsigned char y, unsigned char *texto, unsigned char *alfabeto)
{
	register unsigned char tam, passo = -1;
	unsigned desloc;
	tam = strlen( texto);
	tam--;
	while(!((passo++)==tam))
	{
		if( texto[ passo] > 44 & texto[ passo] < 50)
		{
			desloc = ( texto[ passo] - 1) * 64;
			m_bitmap( x, y - 4, &alfabeto[ desloc]);
		}
		else
		{
			desloc = ( texto[ passo] - 1) * 64;
			m_bitmap( x, y, &alfabeto[ desloc]);
			x = x + 8;
		}
	}
	return( x);
}

int put_textc( int x, unsigned char y, unsigned char *texto, unsigned char *alfabeto, unsigned char cor)
{
	register unsigned char tam, passo = -1;
	unsigned desloc;
	tam = strlen( texto);
	tam--;
	while(!((passo++)==tam))
	{
		if( texto[ passo] > 44 & texto[ passo] < 50)
		{
			desloc = ( texto[ passo] - 1) * 64;
			m_bitmapc( x, y - 4, &alfabeto[ desloc], cor);
		}
		else
		{
			desloc = ( texto[ passo] - 1) * 64;
			m_bitmapc( x, y, &alfabeto[ desloc], cor);
			x = x + 8;
		}
	}
	return( x);
}

int put_text12( int x, unsigned char y, unsigned char *texto, unsigned char *alfabeto)
{
	register unsigned char tam, passo = -1;
	unsigned desloc;
	tam = strlen( texto);
	tam--;
	while(!((passo++)==tam))
	{
		if( texto[ passo] > 44 & texto[ passo] < 50)
		{
			desloc = ( texto[ passo] - 1) * 144;
			m_bitmap12( x, y - 5, &alfabeto[ desloc]);
		}
		else
		{
			desloc = ( texto[ passo] - 1) * 144;
			m_bitmap12( x, y, &alfabeto[ desloc]);
			x = x + 12;
		}
	}
	return( x);
}

int put_text12c( int x, unsigned char y, unsigned char *texto, unsigned char *alfabeto, unsigned char cor)
{
	register unsigned char tam, passo = -1;
	unsigned desloc;
	tam = strlen( texto);
	tam--;
	while(!((passo++)==tam))
	{
		if( texto[ passo] > 44 & texto[ passo] < 50)
		{
			desloc = ( texto[ passo] - 1) * 144;
			m_bitmap12c( x, y - 5, &alfabeto[ desloc], cor);
		}
		else
		{
			desloc = ( texto[ passo] - 1) * 144;
			m_bitmap12c( x, y, &alfabeto[ desloc], cor);
			x = x + 12;
		}
	}
	return( x);
}

char tam_text( unsigned char *texto)
{
	register unsigned char tam, passo = -1;
	int total = 0;
	tam = strlen( texto);
	tam--;
	while(!((passo++)==tam)) if( texto[ passo] < 45 | texto[ passo] > 49) total++;
	return total;
}

void put_peca( int x, int y, struct peca pecas)
{
	bitmap( x, y, &alfa[ 3840 + pecas.primeira * 64 + estilo * 64]);
	bitmap( x, y + 8, &alfa[ 3840 + pecas.segunda * 64 + estilo * 64]);
	bitmap( x, y + 16, &alfa[ 3840 + pecas.terceira * 64 + estilo * 64]);
}

void quadro( void)
{
	register char a = -1;
	while(!((a++)==39))
	{
		bitmap( a * 8, 0, &alfa[ 4352]);
		bitmap( a * 8, 192, &alfa[ 4352]);
	}
	a = 0;
	while(!((a++)==23))
	{
		bitmap( 0, a * 8, &alfa[ 4352]);
		bitmap( 312, a * 8, &alfa[ 4352]);
	}
}

void r_quadro( void)
{
	register unsigned char passoy = 0;
	register unsigned passox = 0, inicial = 2568;
	while(passoy < 184)
	{
		while(passox < 304)
		{
			*(vga_ini + inicial) = 0;
			inicial++;
			passox++;
		}
		passox = 0;
		inicial += 16;
		passoy++;
	}
}

void m_escore( void)
{
	char string[ 8];
	char s = 7;
	long score;
	score = jogo.escore;
	while( 1)
	{
		s--;
		string[ s] = 30 + (score % 10);
		score = score / 10;
		if( score == 0) break;
	}
	string[ 7] = 0;
	barra( 32, 96, 56, 8, 0);
	put_text( 32 + s * 8, 96, &string[ s], alfa);
}

void m_melhor( void)
{
	char string[ 8];
	char s = 7;
	long score;
	score = jogo.melhor;
	while( 1)
	{
		s--;
		string[ s] = 30 + (score % 10);
		score = score / 10;
		if( score == 0) break;
	}
	string[ 7] = 0;
	barra( 32, 160, 56, 8, 0);
	put_text( 32 + s * 8, 160, &string[ s], alfa);
}

void m_proximo( void)
{
	put_peca( 280, 144, peca_prox);
}

void m_pecas( void)
{
	char string[ 8];
	char s = 7;
	int score;
	score = jogo.pecas;
	while( 1)
	{
		s--;
		string[ s] = 30 + (score % 10);
		score = score / 10;
		if( score == 0) break;
	}
	string[ 7] = 0;
	barra( 264, 96, 24, 8, 0);
	put_text( 232 + s * 8, 96, &string[ s], alfa);
}

void m_vidas( void)
{
	char string[ 5];
	char s = -1;
	while(!((s++)==3)) string[ s] = 60;
	string[ 4] = 0;
	s = 4 - jogo.vidas;
	barra( 56, 40, 32, 8, 0);
	put_text( 56 + s * 8, 40, &string[ s], alfa);
}

void m_fase( void)
{
	char string[ 8];
	char s = 7;
	char score;
	score = jogo.fase;
	while( 1)
	{
		s--;
		string[ s] = 30 + (score % 10);
		score = score / 10;
		if( score == 0) break;
	}
	string[ 7] = 0;
	barra( 272, 40, 16, 8, 0);
	put_text( 232 + s * 8, 40, &string[ s], alfa);
}

void g_quadro( void)
{
	register char a = 1;
	unsigned char *point;
	point = &alfa[ 4288];
	r_quadro();
	while(!((a++)==12))
	{
		bitmap( a * 8, 16, point);
		bitmap( a * 8, 56, point);
		bitmap( a * 8, 72, point);
		bitmap( a * 8, 112, point);
		bitmap( a * 8, 128, point);
		bitmap( a * 8, 176, point);
		bitmap( a * 8 + 200, 16, point);
		bitmap( a * 8 + 200, 56, point);
		bitmap( a * 8 + 200, 72, point);
		bitmap( a * 8 + 200, 112, point);
		bitmap( a * 8 + 200, 128, point);
		bitmap( a * 8 + 200, 176, point);
	}
	a = 2;
	while(!((a++)==6))
	{
		bitmap( 16, a * 8, point);
		bitmap( 96, a * 8, point);
		bitmap( 216, a * 8, point);
		bitmap( 296, a * 8, point);
	}
	a = 9;
	while(!((a++)==13))
	{
		bitmap( 16, a * 8, point);
		bitmap( 96, a * 8, point);
		bitmap( 216, a * 8, point);
		bitmap( 296, a * 8, point);
	}
	a = 16;
	while(!((a++)==21))
	{
		bitmap( 16, a * 8, point);
		bitmap( 96, a * 8, point);
		bitmap( 216, a * 8, point);
		bitmap( 296, a * 8, point);
	}
	point = &alfa[ 4416];
	a = 1;
	while(!((a++)==22))
	{
		bitmap( 112, a * 8, point);
		bitmap( 200, a * 8, point);
	}
	a = 14;
	while(!((a++)==24))	bitmap( a * 8, 176, point);
	put_text( 32, 32, escore[ 0], alfa);
	put_text( 32, 88, escore[ 1], alfa);
	put_text( 32, 144, escore[ 2], alfa);
	put_text( 36, 152, escore[ 1], alfa);
	put_text( 232, 32, escore[ 3], alfa);
	put_text( 232, 88, escore[ 4], alfa);
	put_text( 232, 144, escore[ 5], alfa);

	m_vidas();
	m_escore();
	m_melhor();
	m_pecas();
	m_fase();
}


// ******************************************************************
// ***************                                    ***************
// *************** Rotinas de Manipulacao de Arquivos ***************
// ***************                                    ***************
// ******************************************************************


void grava_arquivo( void)
{
	int arc;
	arc = open( "drops2.sav", O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
	write( arc, &opt_cza, 1);
	write( arc, &opt_som, 1);
	write( arc, &opt_eft, 1);
	write( arc, melhores, 160);
	close( arc);
}

void preenche( void)
{
	char t;
	for( t = 0; t < 10; t++)
	{
		melhores[t].dificuldade = b_dificuldade[t];
		melhores[t].n_pontos    = b_pontos[t];
		strcpy( melhores[t].n_jogador, nomes[t]);
	}
	grava_arquivo();
}

void le_arquivo( void)
{
	int arc;
	arc = open( "drops2.sav", O_RDONLY | O_BINARY);
	if( arc == -1)
		preenche();
	else
	{
		read( arc, &opt_cza, 1);
		read( arc, &opt_som, 1);
		read( arc, &opt_eft, 1);
		if( read( arc, melhores, 160) != 160) preenche();
		close( arc);
	}
}

void get_dat( void)
{
	int arc;
	arc = open( "drops2.dat", O_RDONLY | O_BINARY);
	if( arc == -1)
	{
		erro_arq = 1;
		exit( 1);
	}
	if( (unsigned int) read( arc, alfa, 39424) != 39424)
	{
		erro_arq = 1;
		exit( 1);
	}
	close( arc);
}


// Rotina extra que n„o pode ser colocada mais abaixo


int lbuffer( void)
{
	int rt = 0;
	while( kbhit())
	{
		rt = 1;
		getch();

	}
	return rt;
}


// ******************************************************************
// ***************                                    ***************
// *************** Biblioteca de Manipulacao de Tempo ***************
// ***************                                    ***************
// ******************************************************************


long tempo_antigo, tempo_fixado;
char extrapola;

long get_time( void)
{
	return( hora_real);
}

void reseta( int tempo)
{
	extrapola = 0;
	tempo_antigo = get_time();
	tempo_fixado = tempo_antigo + tempo;
	if(tempo_fixado > 65535L)
	{
		tempo_fixado -= 65536L;
		extrapola = 1;
	}
}

void time_delay( int tempo)
{
	long tempo_atual;
	while( 1)
	{
		tempo_atual = get_time();
		if( extrapola == 0)
		{
			if( ( tempo_atual < tempo_antigo) | ( tempo_atual >= tempo_fixado))
			{
				reseta( tempo);
				return;
			}
		}
		else
		{
			if( ( tempo_atual < tempo_antigo) & ( tempo_atual >= tempo_fixado))
			{
				reseta( tempo);
				return;
			}
		}
	}
}

void time_delay2( int tempo)
{
	long tempo_atual;
	while( 1)
	{
		lbuffer();
		tempo_atual = get_time();
		if( extrapola == 0)
		{
			if( ( tempo_atual < tempo_antigo) | ( tempo_atual >= tempo_fixado))
			{
				reseta( tempo);
				return;
			}
		}
		else
		{
			if( ( tempo_atual < tempo_antigo) & ( tempo_atual >= tempo_fixado))
			{
				reseta( tempo);
				return;
			}
		}
	}
}

int time_loop( void)
{
	long tempo_atual;
	tempo_atual = get_time();
	if( extrapola == 0)
	{
		if( ( tempo_atual < tempo_antigo) | ( tempo_atual >= tempo_fixado))
			return( 1);
	}
	else
	{
		if( ( tempo_atual < tempo_antigo) & ( tempo_atual >= tempo_fixado))
			return( 1);
	}
	return( 0);
}

void espere( int tempo)
{
	reseta( tempo);
	time_delay2( tempo);
}


// *****************************************************************
// ***************                                   ***************
// *************** Biblioteca de Manipulacao de Sons ***************
// ***************                                   ***************
// *****************************************************************


void seta_som_mudo( void)
{
	estado_som = 0;
}

void seta_musica( int far *nova_musica, int novo_tam)
{
	contador_musica = 0;
	tam_musica = novo_tam;
	pt_musica = nova_musica;
	estado_som = 1;
}

void seta_efeito( int far *novo_efeito, int novo_tam)
{
	if( estado_som != 2)
		estado_antigo = estado_som;
	contador_efeito = 0;
	tam_efeito = novo_tam;
	pt_efeito = novo_efeito;
	estado_som = 2;
}


// ****************************************
// *************** Programa ***************
// ****************************************


void mostra_staff( void)
{
	register unsigned char passo = -1, tam;
	lbuffer();
	reseta( 64);
	while(!((passo++)==5))
	{
		tam = tam_text( staff[ passo * 3]);
		put_text( 160 - 4 * tam, 60, staff[ passo * 3], simp);
		while(!(time_loop())) if( lbuffer()) return;
		reseta( 32);
		tam = tam_text( staff[ passo * 3 + 1]);
		put_text12c( 160 - 6 * tam, 84, staff[ passo * 3 + 1], alfa12, 243);
		while(!(time_loop())) if( lbuffer()) return;
		reseta( 160);
		tam = tam_text( staff[ passo * 3 + 2]);
		put_text12c( 160 - 6 * tam, 104, staff[ passo * 3 + 2], alfa12, 243);
		while(!(time_loop())) if( lbuffer()) return;
		reseta( 64);
		barra( 28, 56, 264, 60, 0);
	}
}

void mostra_melhores( void)
{
	char t,s;
	long temp;
	r_quadro();
	reseta( 640);
	lbuffer();
	put_text12( 40, 19, highmess, alfa12);
	for( t=0; t<10; t++)
	{
		put_text12c( 24, 43+t*14, melhores[t].n_jogador, alfa12, 75+t*12);
		temp = melhores[t].n_pontos;
		for( s=6; s>0; s--)
		{
			nronovo[s-1] = 30+(temp%10);
			temp/=10;
		}
		put_text12c( 156, 43+t*14, nronovo, alfa12, 75+t*12);
		s = melhores[t].dificuldade;
		put_textc( 248-s*4, 47+t*14, opcoes[3+s], alfa, 75+t*12);
	}
	while( time_loop() == 0)
		if( lbuffer() ) return;
}

void pede_nome( char ver)
{
	char n_char=0,t=1;
	int x;
	r_quadro();
	lbuffer();
	strcpy( editar, "((((((((((");
	put_text12( 76, 28, digite[0], alfa12);
	put_text12( 40, 48, highmess, alfa12);
	put_text12c( 64, 84, digite[1], alfa12, 243);
	put_text12c( 100, 112, editar, alfa12, 75+ver*12);
	while( t)
	{
		x=getch();
		if( x == 0)
		{
			x=getch();
			x=0;
		}
		if( x>96 && x<123) x-=32;
		if( x>64 && x<91)
		{
			if( n_char<10) n_char++;
			barra( 88+n_char*12,112,12,12,0);
			editar[n_char-1]=x-63;
			put_text12c( 100, 112, editar, alfa12, 75+ver*12);
		}
		if( x==8 && n_char>0)
		{
			n_char--;
			barra( 100+n_char*12,112,12,12,0);
			editar[n_char]=40;
			put_text12c( 100, 112, editar, alfa12, 75+ver*12);
		}
		if( x == 13) t=0;
	}
	strcpy( melhores[ver].n_jogador, editar);
}

void verifica_recorde( void)
{
	char ver=10,t;
	for( t=9; t>=0; t--)
		if( melhores[t].n_pontos < jogo.escore) ver = t;
	if( ver < 9)
		for( t=9; t>ver; t--)
			melhores[t]=melhores[t-1];
	if( ver < 10)
	{
		pede_nome(ver);
		melhores[ver].dificuldade=dif_opt;
		melhores[ver].n_pontos=jogo.escore;
		grava_arquivo();
		mostra_melhores();
		seta_som_mudo();
	}
}

void zera_matriz( void)
{
	register char a = -1, b = 0;
	while(!((a++)==19))
	{
		while(!((b++)==10)) fosso[ b][ a] = 8;
		b = 0;
	}
}

void init_matriz( void)
{
	register char a = -1, b = -1;
	while(!((a++)==20))
	{
		while(!((b++)==11)) fosso[ b][ a] = 7;
		b = -1;
	}
	zera_matriz();
}

void seta_18( void)
{
	outportb( 0x41, 0xB6);
	outportb( 0x40, 0xFF);
	outportb( 0x40, 0xFF);
}

void seta_64( void)
{
	outportb( 0x41, 0xB6);
	outportb( 0x40, 0xD3);
	outportb( 0x40, 0x48);
}

void inicializa( void)
{
	register char a = -1;

	if( detecta_vga() == 0 )
	{
		sem_placa = 1;
		exit( 1);
	}

	while(!((a++)==17)) converte( staff[ a]);
	a = -1;
	while(!((a++)==5)) converte( escore[ a]);
	a = -1;
	while(!((a++)==3)) converte( menu[ a]);
	a = -1;
	while(!((a++)==9)) converte( opcoes[ a]);
	a = -1;
	while(!((a++)==9)) converte( nomes[ a]);
	a = -1;
	while(!((a++)==1)) converte( digite[ a]);
	a = -1;
	while(!((a++)==2)) converte( config[ a]);
	a = -1;
	while(!((a++)==1)) converte( simnao[ a]);
	a = -1;

	while(!((a++)==3))
	{
		estado_tecla[ a] = 0;
		press_tecla[ a]  = 0;
	}

	le_arquivo();

	init_matriz();
	converte( mensagem);
	converte( gameover);
	converte( highmess);
	converte( versaobeta);
	jogo.proximo = 1;
	jogo.melhor = melhores[0].n_pontos;

	alfa = malloc( 39424);
	if( alfa==NULL)
	{
		sem_memo=0;
		exit(1);
	}
	zicau = &alfa[ 8512];
	simp = &zicau[ 2048];
	alfa12 = &simp[ 3840];
	dropsii = &alfa12[ 8640];

	get_dat();

	randomize();
	vga();
	palheta();
	if( opt_cza==0) gray_scale();

	seta_64();
	velha_int_09h = getvect( 0x09);
	velha_int_1ch = getvect( 0x1c);
	velha_int_1bh = getvect( 0x1b);
	troquei_ints = 1;
	setvect( 0x1c, nova_int_1ch);
	setvect( 0x09, nova_int_09h);
	setvect( 0x1b, nova_int_1bh);
}

void m_zicau( void)
{
	register char a = -1, b = -1;
	while(!((a++)==3))
	{
		while(!((b++)==7))
		{
			bitmap( 128 + b * 8, 56 + a * 8, &zicau[ (b + a * 8) * 64]);
		}
		b = -1;
	}
	reseta( 128);
	time_delay( 6);
	cor( 56, 0, 0, 0);
	put_text( 125, 104, mensagem, alfa);
	a = -1;
	while(!((a++)==6))
	{
		b = 8 * ( a + 1);
		cor( 56, b, b, b);
		time_delay( 6);
	}
	espere( 128);
	a = -1;
	reseta( 3);
	while(!((a++)==11))
	{
		b = 6 - a;
		if( b < 0) b = 0;
		sobe( b);
		b = 13 + a;
		if( b == 24) b = 23;
		desce( b);
		time_delay( 3);
	}
}

char bateu( unsigned char x, char y)
{
	if( fosso[ x][ y + 3] == 8) return( 0);
	return( 1);
}

void cola( unsigned char x, unsigned char y)
{
	fosso[ x][ y] = peca1.primeira;
	fosso[ x][ y + 1] = peca1.segunda;
	fosso[ x][ y + 2] = peca1.terceira;
}

void tire( unsigned int x, unsigned char y)
{
	barra( x, y, 8, 24, 0);
}

void tir1( unsigned int x, unsigned char y)
{
	barra( x, y, 8, 8, 0);
}

void sorteia( struct peca *p_peca)
{
	p_peca[ 0].primeira = random( 7);
	while( ( p_peca[ 0].segunda = random( 7) ) == p_peca[ 0].primeira );
	while( ( ( p_peca[ 0].terceira = random( 7) ) == p_peca[ 0].primeira )
		 || p_peca[ 0].terceira == p_peca[ 0].segunda );
}

void vira( void)
{
	char temp;
	temp = peca1.terceira;
	peca1.terceira = peca1.segunda;
	peca1.segunda = peca1.primeira;
	peca1.primeira = temp;
}

void d_teclas( void)
{
	char y;
	if( press_tecla[ 0] == 1)
	{
		menu_opt = menu_opt - 1 + (menu_opt==0) * 4;
		for( y=0; y<4; y++)
			put_text12c( 124, 100+20*y, menu[ y], alfa12, 81+6*(menu_opt==y));
		press_tecla[ 0] = 0;
		reseta( 640);
	}
	if( press_tecla[ 2] == 1)
	{
		menu_opt = menu_opt + 1 - (menu_opt==3) * 4;
		for( y=0; y<4; y++)
			put_text12c( 124, 100+20*y, menu[ y], alfa12, 81+6*(menu_opt==y));
		press_tecla[ 2] = 0;
		reseta( 640);
	}
}

void m_dropsii( void)
{
	unsigned char y;
	int x, desvio = 5152;
	unsigned int d_desvio = 0;
	lbuffer();
	press_tecla[ 2] = 0;
	press_tecla[ 0] = 0;
	deixou_tempo    = 0;
	for( y=0; y<64; y++)
	{
		for( x=0; x<256; x++)
		{
			*(vga_ini + desvio) = dropsii[ d_desvio];
			d_desvio++;
			desvio++;
		}
		desvio+=64;
	}

	put_text12c( 132, 68, versaobeta, alfa12, 111);

	for( x=9; x<31; x++)
	{
		bitmap( x*8,  88, &alfa[ 4288]);
		bitmap( x*8, 176, &alfa[ 4288]);
	}

	for( y=12; y<22; y++)
	{
		bitmap( 106, y*8, &alfa[ 4288]);
		bitmap( 206, y*8, &alfa[ 4288]);
		bitmap( 104, y*8, &alfa[ 4288]);
		bitmap( 208, y*8, &alfa[ 4288]);
	}

	for( y=0; y<4; y++)
		put_text12c( 124, 100+20*y, menu[ y], alfa12, 81+6*(menu_opt==y));
	y = 1;
	reseta( 640);
	while( y)
	{
		if( kbhit())
		{
			x = getch();
			if( x == 13 || x == 32) y = 0;
			reseta( 640);
		}
		d_teclas();
		if( time_loop() == 1)
		{
			mostra_melhores();
			seta_som_mudo();
			deixou_tempo = 1;
			y = 0;
		}
	}
}

char get_estilo( char novo_estilo)
{
	if( novo_estilo == 0) return( 0);
	return( 3 + 7 * novo_estilo);
}

void m_t_pec( char nro)
{
	char n_est, y;
	n_est = get_estilo( nro);
	if( nro == pec_opt) barra( 125+16*nro, 117, 14, 62, 16);
		else barra( 125+16*nro, 117, 14, 62, 0);
	for( y = 0; y < 7; y++)
		m_bitmap( 128+16*nro, 120+8*y, &alfa[ 3840 + (y+n_est) * 64]);
}


void o_teclas( void)
{
	char y;
	if( press_tecla[ 0] == 1)
	{
		m_o_opt = m_o_opt - 1 + (m_o_opt==0) * 3;
		for( y=0; y<3; y++)
			put_text12c( m_o_x[y], 24+48*y, opcoes[y], alfa12, 81+6*(m_o_opt==y));
		press_tecla[ 0] = 0;
		press_tecla[ 3] = 0;
		press_tecla[ 1] = 0;
	}
	if( press_tecla[ 2] == 1)
	{
		m_o_opt = m_o_opt + 1 - (m_o_opt==2) * 3;
		for( y=0; y<3; y++)
			put_text12c( m_o_x[y], 24+48*y, opcoes[y], alfa12, 81+6*(m_o_opt==y));
		press_tecla[ 2] = 0;
		press_tecla[ 3] = 0;
		press_tecla[ 1] = 0;
	}
	if( m_o_opt == 0)
	{
		if( press_tecla[ 1] == 1)
		{
			dif_opt = dif_opt + 1 - (dif_opt==2) * 3;
			for( y=0; y<3; y++)
				put_text12c( m_o_y[y], 44, opcoes[y+3], alfa12, 237+6*(dif_opt==y));
			press_tecla[ 1] = 0;
		}
		if( press_tecla[ 3] == 1)
		{
			dif_opt = dif_opt - 1 + (dif_opt==0) * 3;
			for( y=0; y<3; y++)
				put_text12c( m_o_y[y], 44, opcoes[y+3], alfa12, 237+6*(dif_opt==y));
			press_tecla[ 3] = 0;
		}
	}
	if( m_o_opt == 1)
	{
		if( press_tecla[ 1] == 1)
		{
			vid_opt = vid_opt + 1 - (vid_opt==4) * 4;
			for( y=0; y<4; y++)
				put_text12c( m_o_xy[y], 92, opcoes[y+6], alfa12, 71+76*(vid_opt==y+1));
			press_tecla[ 1] = 0;
		}
		if( press_tecla[ 3] == 1)
		{
			vid_opt = vid_opt - 1 + (vid_opt==1) * 4;
			for( y=0; y<4; y++)
				put_text12c( m_o_xy[y], 92, opcoes[y+6], alfa12, 71+76*(vid_opt==y+1));
			press_tecla[ 3] = 0;
		}
	}
	if( m_o_opt == 2)
	{
		if( press_tecla[ 1] == 1)
		{
			pec_opt = pec_opt + 1 - (pec_opt==9) * 10;
			m_t_pec( pec_opt);
			m_t_pec( ant_pec_opt);
			estilo = get_estilo( pec_opt);
			ant_pec_opt = pec_opt;
			press_tecla[ 1] = 0;
		}
		if( press_tecla[ 3] == 1)
		{
			pec_opt = pec_opt - 1 + (pec_opt==0) * 10;
			m_t_pec( pec_opt);
			m_t_pec( ant_pec_opt);
			estilo = get_estilo( pec_opt);
			ant_pec_opt = pec_opt;
			press_tecla[ 3] = 0;
		}
	}
}

void cfg_tec( void)
{
	char y;
	if( press_tecla[ 0] == 1)
	{
		opt_cfg = opt_cfg - 1 + (opt_cfg==0) * 3;
		for( y=0; y<3; y++)
			put_text12c( m_cfg[y], 24+58*y, config[y], alfa12, 81+6*(opt_cfg==y));
		press_tecla[ 0] = 0;
		press_tecla[ 3] = 0;
		press_tecla[ 1] = 0;
	}
	if( press_tecla[ 2] == 1)
	{
		opt_cfg = opt_cfg + 1 - (opt_cfg==2) * 3;
		for( y=0; y<3; y++)
			put_text12c( m_cfg[y], 24+58*y, config[y], alfa12, 81+6*(opt_cfg==y));
		press_tecla[ 2] = 0;
		press_tecla[ 3] = 0;
		press_tecla[ 1] = 0;
	}
	if( press_tecla[ 3] == 1)
	{
		c_cfg[opt_cfg] = c_cfg[opt_cfg] - 1 + (c_cfg[opt_cfg]==0) * 2;
		for( y=0; y<2; y++)
			put_text12c( 112+60*y, 44+58*opt_cfg, simnao[y], alfa12, 237+6*(c_cfg[opt_cfg]==y));
		press_tecla[ 3] = 0;
	}
	if( press_tecla[ 1] == 1)
	{
		c_cfg[opt_cfg] = c_cfg[opt_cfg] + 1 - (c_cfg[opt_cfg]==1) * 2;
		for( y=0; y<2; y++)
			put_text12c( 112+60*y, 44+58*opt_cfg, simnao[y], alfa12, 237+6*(c_cfg[opt_cfg]==y));
		press_tecla[ 1] = 0;
	}
}

void m_opcoes( void)
{
	unsigned char y;
	int x;

	lbuffer();
	r_quadro();
	for( y=0; y<4; y++) press_tecla[ y] = 0;
	for( y=0; y<3; y++)
		put_text12c( m_o_x[y], 24+48*y, opcoes[y], alfa12, 81+6*(m_o_opt==y));
	for( y=0; y<3; y++)
		put_text12c( m_o_y[y], 44, opcoes[y+3], alfa12, 237+6*(dif_opt==y));
	for( y=0; y<4; y++)
		put_text12c( m_o_xy[y], 92, opcoes[y+6], alfa12, 71+76*(vid_opt==y+1));
	for( y=0; y<10; y++)
		m_t_pec( y);
	y = 1;
	while( y)
	{
		if( kbhit())
		{
			x = getch();
			if( x == 13 || x == 32) y = 0;
		}
		o_teclas();
	}
}

void m_config( void)
{
	unsigned char y;
	int x;

	c_cfg[0] = opt_cza;
	c_cfg[1] = opt_som;
	c_cfg[2] = opt_eft;
	lbuffer();
	r_quadro();
	for( y=0; y<4; y++) press_tecla[ y] = 0;
	for( y=0; y<3; y++)
	{
		put_text12c( m_cfg[y], 24+58*y, config[y], alfa12, 81+6*(opt_cfg==y));
		for( x=0; x<2; x++)
			put_text12c( 112+60*x, 44+58*y, simnao[x], alfa12, 237+6*(c_cfg[y]==x));
	}
	y = 1;
	while( y)
	{
		if( kbhit())
		{
			x = getch();
			if( x == 13 || x == 32)
			{
				r_quadro();
				if( c_cfg[0] != opt_cza)
					if( c_cfg[0] == 0)
						gray_scale();
					else
						palheta_cor();
				opt_cza = c_cfg[0];
				opt_som = c_cfg[1];
				opt_eft = c_cfg[2];
				grava_arquivo();
				y = 0;
			}
		}
		cfg_tec();
	}
}

void m_gameover( void)
{
	lbuffer();
	r_quadro();
	put_text12c( 160 - 6 * tam_text( gameover), 80, gameover, alfa12, 243);
	reseta( 192);
	while(!(time_loop()))
		if( kbhit())
		{
			lbuffer();
			return;
		}
}

void m_fimdejogo( void)
{
	// Fim do jogo Drops II.
}

void i_round( void)
{
	char string[ 8];
	char s = 7, r = 1;
	char score;
	r_quadro();
	put_text12c( 136, 66, escore[ 3], alfa12, 111);
	score = jogo.fase;
	while( 1)
	{
		s--;
		string[ s] = 30 + (score % 10);
		score = score / 10;
		if( score == 0) break;
	}
	string[ 7] = 0;
	if( jogo.fase > 9) r = 2;
	put_text12c( 160 - r * 6, 94, &string[ s], alfa12, 147);
	espere( 96);
	lbuffer();
}

int desca( void)
{
	register unsigned char t = -1, r = -1;
	unsigned char s;
	int total = 0;
	while(!((r++)==9))
	{
		while(!((t++)==19)) ftemp[ r][ t] = fosso[ r + 1][ t];
		t = -1;
	}
	t = -1;
	r = -1;
	while(!((r++)==7))
	{
		while(!((t++)==19))
		{
			if( fosso[ r + 1][ t] == fosso[ r + 2][ t] &&
				fosso[ r + 1][ t] == fosso[ r + 3][ t] &&
				fosso[ r + 1][ t] < 8)
			{
				ftemp[ r][ t] = 9;
				ftemp[ r + 1][ t] = 9;
				ftemp[ r + 2][ t] = 9;
				barra( 120 + r * 8, 16 + t * 8, 24, 8, 16);
			}
		}
		t = -1;
	}
	t = -1;
	r = -1;
	while(!((r++)==9))
	{
		while(!((t++)==17))
		{
			if( fosso[ r + 1][ t] == fosso[ r + 1][ t + 1] &&
				fosso[ r + 1][ t] == fosso[ r + 1][ t + 2] &&
				fosso[ r + 1][ t] < 8)
			{
				ftemp[ r][ t] = 9;
				ftemp[ r][ t + 1] = 9;
				ftemp[ r][ t + 2] = 9;
				barra( 120 + r * 8, 16 + t * 8, 8, 24, 16);
			}
		}
		t = -1;
	}
	t = -1;
	r = -1;
	while(!((r++)==7))
	{
		while(!((t++)==17))
		{
			if( fosso[ r + 1][ t] == fosso[ r + 2][ t + 1] &&
				fosso[ r + 1][ t] == fosso[ r + 3][ t + 2] &&
				fosso[ r + 1][ t] < 8)
			{
				ftemp[ r][ t] = 9;
				ftemp[ r + 1][ t + 1] = 9;
				ftemp[ r + 2][ t + 2] = 9;
				barra( 120 + r * 8, 16 + t * 8, 8, 8, 16);
				barra( 128 + r * 8, 24 + t * 8, 8, 8, 16);
				barra( 136 + r * 8, 32 + t * 8, 8, 8, 16);
			}
		}
		t = -1;
	}
	t = 1;
	r = -1;
	while(!((r++)==7))
	{
		while(!((t++)==19))
		{
			if( fosso[ r + 1][ t] == fosso[ r + 2][ t - 1] &&
				fosso[ r + 1][ t] == fosso[ r + 3][ t - 2] &&
				fosso[ r + 1][ t] < 8)
			{
				ftemp[ r][ t] = 9;
				ftemp[ r + 1][ t - 1] = 9;
				ftemp[ r + 2][ t - 2] = 9;
				barra( 120 + r * 8, 16 + t * 8, 8, 8, 16);
				barra( 128 + r * 8, 8 + t * 8, 8, 8, 16);
				barra( 136 + r * 8, t * 8, 8, 8, 16);
			}
		}
		t = 1;
	}

	t = -1;
	r = -1;
	while(!((r++)==9))
	{
		while(!((t++)==19))	if( ftemp[ r][ t] == 9) total++;
		t = -1;
	}
	if( total == 0) return( 0);
	t = -1;
	r = -1;
	while(!((r++)==9))
	{
		while(!((t++)==19))
		{
			if( ftemp[ r][ t] == 9)
			{
				s = t + 1;
				if( s > 1)
				{
					while(!((s--)==1)) ftemp[ r][ s] = ftemp[ r][ s - 1];
				}
				ftemp[ r][ 0] = 8;
			}
		}
		t = -1;
	}
	if (!opt_eft) seta_efeito(efect,4);
	espere( 20);
	t = -1;
	r = -1;
	while(!((r++)==9))
	{
		while(!((t++)==19))
		{
			if( ftemp[ r][ t] < 8)
			bitmap( 120 + r * 8, 16 + t * 8, &alfa[ 3840 + ftemp[ r][ t] * 64 + estilo * 64]);
			else barra( 120 + r * 8, 16 + t * 8, 8, 8, 0);
			fosso[ r + 1][ t] = ftemp[ r][ t];
		}
		t = -1;
	}
	reseta( jogo.passo);
	acum += total;
	return( 1);
}

char teclas( void)
{
	char pont;
	char a1 = NULL;
	while( kbhit())
	{
		a1 = getch();
	}
	if( a1 == 32)
	{
		pont = y_peca;
		while( 1)
		{
			if( bateu( x_peca, pont)) break;
			pont++;
		}
		tire( 112 + x_peca * 8, 16 + y_peca * 8);
		y_peca = pont;
		put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
		reseta( 0);
	}
	if( a1 == 27) return( 1);
	if( a1 == 13)
	{
		jogo.proximo = 1 + ( jogo.proximo == 1);
		if( jogo.proximo == 1) m_proximo();
		else barra( 280, 144, 8, 24, 0);
	}
	if( press_tecla[3] == 1)
	{
		pont = x_peca - 1;
		if( bateu( pont, y_peca - 1) == 0)
		{
			tire( 112 + x_peca * 8, 16 + y_peca * 8);
			x_peca = pont;
			put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
		}
		press_tecla[3] = 0;
	}
	if( press_tecla[1]  == 1)
	{
		pont = x_peca + 1;
		if( bateu( pont, y_peca - 1) == 0)
		{
			tire( 112 + x_peca * 8, 16 + y_peca * 8);
			x_peca = pont;
			put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
		}
		press_tecla[1] = 0;
	}
	if( press_tecla[2] == 1)
	{
		vira();
		put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
		press_tecla[2] = 0;
	}
	if( press_tecla[0] == 1)
	{
		vira();
		vira();
		put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
		press_tecla[0] = 0;
	}
	return( 0);
}

char rjogo( void)
{
	char zerou = 1, tocou = 1;

	jogo.fase = 0;
	jogo.escore = 0;
	jogo.vidas = vid_opt;
	sorteia( &peca_prox);
	while( 1)
	{
		seta_som_mudo();
		zera_matriz();
		jogo.fase++;
		if( jogo.fase > 10) return( 0);
		jogo.pecas = 100 + 100 * ( dif_opt == 2);
		jogo.passo = ( 44 - jogo.fase * 4) * ( 1 + 1 * ( dif_opt == 0));
		i_round();
		g_quadro();
		press_tecla[0]=0;
		press_tecla[1]=0;
		press_tecla[2]=0;
		press_tecla[3]=0;
		reseta( jogo.passo);
		while( zerou)
		{
			peca1 = peca_prox;
			sorteia( &peca_prox);
			x_peca = 5;
			y_peca = 0;
			if( jogo.proximo == 1) m_proximo();
			if(!bateu( x_peca, y_peca - 1))
			{
				while( tocou)
				{
					put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
					while(!(time_loop())) if( teclas()) return( 2);
					reseta( jogo.passo);
					if( bateu( x_peca, y_peca))
					{
						tocou = 0;
						cola( x_peca, y_peca);
						acum = 0;
						while(desca());
						if( acum > 0)
						{
							jogo.pecas -= acum;
							if( jogo.pecas < 1)
							{
								jogo.pecas = 0;
								zerou = 0;
							}
							m_pecas();
							jogo.escore += acum * 200 - 300;
							if( jogo.escore > jogo.melhor)
							{
								jogo.melhor = jogo.escore;
								m_melhor();
							}
							m_escore();
						}
						else if (!opt_eft) seta_efeito(&efect[3],2);
					}
					else
					{
						tir1( 112 + x_peca * 8, 16 + y_peca * 8);
						y_peca++;
					}
				}
				tocou = 1;
			}
			else
			{
				reseta(32);
				put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
				time_delay2( 6);
				tire( 112 + x_peca * 8, 16 + y_peca * 8);
				time_delay2( 32);
				put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
				time_delay2( 6);
				tire( 112 + x_peca * 8, 16 + y_peca * 8);
				time_delay2( 32);
				put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
				time_delay2( 6);
				tire( 112 + x_peca * 8, 16 + y_peca * 8);
				time_delay2( 32);
				put_peca( 112 + x_peca * 8, 16 + y_peca * 8, peca1);
				time_delay2( 6);
				if( jogo.vidas == 1)
				{
					seta_som_mudo();
					return( 1);
				}
				zera_matriz();
				jogo.vidas--;
				g_quadro();
				lbuffer();
			}
			lbuffer();
		}
		zerou = 1;
	}
}

void fim_do_programa( void )
{
	seta_som_mudo();
	nosound();
	seta_18();
	if( troquei_ints != 0 )
	{
		setvect( 0x09, velha_int_09h);
		setvect( 0x1c, velha_int_1ch);
		setvect( 0x1b, velha_int_1bh);
	}
	closevga();
	textmode( 3);
	*end_teclado = 0;
	if( sem_memo !=0) printf( "\nDROPS II precisa de 130 Kbytes de mem¢ria para rodar.\n\n");
	if( erro_arq !=0) printf( "\nErro na leitura ou arquivo n„o encontrado!\n\n");
	if( sem_placa != 0) printf( "\nSua placa de v¡deo n„o suporta o modo gr fico 320x200 com 256 cores.\n\n");
	if( sem_memo==0 && erro_arq==0 && sem_placa==0)
		printf( "\nDROPS II - ZICAUSOFT 1995 - Obrigado por jogar.\n\n");
	delay( 1000);
}

void main()
{
	char sai_jogo=1;
	char my_var;

	atexit( fim_do_programa);

	inicializa();

	m_zicau();

	reseta( 96);
	time_delay( 0);
	quadro();

	mostra_staff();

	seta_som_mudo();

	while( sai_jogo)
	{
		r_quadro();
		m_dropsii();
		if( deixou_tempo == 0)
		{
			if( menu_opt == 3) sai_jogo = 0;
			if( menu_opt == 1) m_opcoes();
			if( menu_opt == 2) m_config();
			if( menu_opt == 0)
			{
				my_var = rjogo();
				verifica_recorde();
				if( my_var == 1) m_gameover();
				if( my_var == 0) m_fimdejogo();
				seta_som_mudo();
			}
		}
	}
}
