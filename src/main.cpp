 #include <allegro.h> 
 #include <stdlib.h>
 
 #include "constantes.h"

 #define MAX_X         640
 #define MAX_Y         480
 #define V_MAX_X       0
 #define V_MAX_Y       0
 #define COLOR_BITS    16
 
 #define DELAY_GIRO    15
 
	typedef struct {
		int px;
		int py;
		int vx;
		int vy;
		int pintar;
	} Bala;
 
	typedef struct {
		Bala **balas1;
		BITMAP *bitbal1;
		Bala **balas2;
		BITMAP *bitbal2;
		Bala **balas3;
		BITMAP *bitbal3;
		BITMAP **sprites;
		BITMAP * bitmap;
		BITMAP **explosao;
		int pxb1;
		int pyb1;
		int pxb2;
		int pyb2;
		int pxb3;
		int pyb3;
		int danob1;
		int danob2;
		int danob3;
		int px;
		int py;
		int vx;
		int vy;
		int direcao;
		int cadencia;
		int delayTiro;
		int numb;
		int acel;
		int expl;
		int pxexp;
		int pyexp;
		int wexp;
		int hexp;
		int pintar;
	} Nave;
	
	typedef struct {
		BITMAP * bitmap;
		int vy;
		int py;
	} Scroll;

	 typedef struct {
		Nave **navesAliadas;
		Nave **navesInimigas;
	} Tela;
 
	Scroll * carregarScroll (char * nome, PALLETE pal) {
		Scroll * retorno = (Scroll *) malloc (sizeof (Scroll));
		retorno->vy = 0;
		retorno->py = 0;
		retorno->bitmap = load_bitmap(nome, pal);
		return retorno;
	}
	
	void carregarSpritesNave(Nave *n, BITMAP *base,int escolha) {
		int x = escolha*200;
		n->sprites =(BITMAP **)calloc(sizeof(BITMAP *), 10);
		int i;
		for(i = 0; i < 10; i++) {
		n->sprites[i] = create_sub_bitmap(base, x, 0, 20, 31);
		x = x + 20;
		}
		
		n->explosao =(BITMAP **)calloc(sizeof(BITMAP *), 10); 

		n->balas1 = (Bala **)calloc(sizeof(Bala *), 20);
		n->balas2 = (Bala **)calloc(sizeof(Bala *), 20);
		n->balas3 = (Bala **)calloc(sizeof(Bala *), 20);
		
		for(i = 0; i < 20; i++) {
			Bala * b1;
			b1 = (Bala *) malloc (sizeof (Bala));
			b1->px = 0;
			b1->py = 0;
			b1->vx = 0;
			b1->vy = 0;
			b1->pintar = 0;
			Bala * b2;
			b2 = (Bala *) malloc (sizeof (Bala));
			b2->px = 0;
			b2->py = 0;
			b2->vx = 0;
			b2->vy = 0;
			b2->pintar = 0;
			Bala * b3;
			b3 = (Bala *) malloc (sizeof (Bala));
			b3->px = 0;
			b3->py = 0;
			b3->vx = 0;
			b3->vy = 0;
			b3->pintar = 0;
			n->balas1[i] = b1;
			n->balas2[i] = b2;
			n->balas3[i] = b3;
		}
	}
	
	Nave * carregarNave (char * nave, int escolha) {
		Nave * retorno = (Nave *) malloc  (sizeof (Nave));
		BITMAP * nav = load_bitmap(nave, NULL);
		carregarSpritesNave(retorno, nav, escolha);
		retorno->px = 0;
		retorno->py = 0;
		retorno->vx = 0;
		retorno->vy = 0;
		retorno->pxb1 = 0;
		retorno->pyb1 = 0;
		retorno->pxb2 = 0;
		retorno->pyb2 = 0;
		retorno->pxb3 = 0;
		retorno->pyb3 = 0;
		retorno->danob1 = 0;
		retorno->danob2 = 0;
		retorno->danob3 = 0;
		retorno->direcao = 0;
		retorno->cadencia = 0;
		retorno->delayTiro = 0;
		retorno->numb = 0;
		retorno->acel = 0;
		retorno->expl = 0;
		retorno->pxexp = 0;
		retorno->pyexp = 0;
		retorno->wexp = 0;
		retorno->hexp = 0;
		retorno->pintar = 0;
		retorno->bitmap = retorno->sprites[0];
		return retorno;
	}
	
	Tela * carregarTela () {
		Tela * retorno = (Tela *) malloc (sizeof (Tela));
		retorno->navesAliadas = (Nave **)calloc(sizeof(Nave *), 4);
		retorno->navesInimigas = (Nave **)calloc(sizeof(Nave *), 30);  
		return retorno;
	}
	
	void moverScroll (Scroll * s1, Scroll * s2, int delay, int tempo) {
		if(tempo%delay == 0) {
			s1->py = s1->py + s1->vy;
			s2->py = s2->py + s2->vy;
		}
		if(s1->py > 600) s1->py = -1800;
		if(s2->py > 600) s2->py = -1800;
	}
	
	void setScroll(Scroll *s, int py, int vy) {
		s->py = py;
		s->vy = vy;
	}
	
	void moverNave(Nave *n, int up, int down, int left, int right, int tempo) {
		if(tempo%n->acel == 0) {
			if(up) {
				n->direcao = 0;
				n->py -= n->vy;
				if(n->py <= V_MAX_Y) n->py = V_MAX_Y;
			}if(down) {
				n->direcao = 0;
				n->py += n->vy;
				if(n->py >= MAX_Y - n->bitmap->h) n->py = MAX_Y - n->bitmap->h;
				}
		}if(tempo%n->acel == 0) {  
			if(left) {
				if(n->direcao > -1) n->direcao = -1;
				else if(n->direcao > -DELAY_GIRO) n->direcao--;
				n->px -= n->vx;
				if(n->px <= V_MAX_X) n->px = V_MAX_X;
			}if(right) {
				if(n->direcao < 1) n->direcao = 1;
				else if(n->direcao < DELAY_GIRO) n->direcao++;
				n->px += n->vx;
				if(n->px >= MAX_X - n->bitmap->h) n->px = MAX_X - n->bitmap->h;
			}else if(!up && !down && !left && !right) n->direcao = 0;
		}
	}
	
	void moverNaveInimiga(Nave *n, int up, int down, int left, int right, int tempo) {
		if(tempo%n->acel == 0) {
			if(up) {
				n->direcao = 0;
				n->py -= n->vy;
			}if(down) {
				n->direcao = 0;
				n->py += n->vy;
				}
		}if(tempo%n->acel == 0) {  
			if(left) {
				if(n->direcao > -1) n->direcao = -1;
				else if(n->direcao > -DELAY_GIRO) n->direcao--;
				n->px -= n->vx;
			}if(right) {
				if(n->direcao < 1) n->direcao = 1;
				else if(n->direcao < DELAY_GIRO) n->direcao++;
				n->px += n->vx;
			}else if(!up && !down && !left && !right) n->direcao = 0;
		}
	}
	
	void atualizarNave(Nave *n, int tempo) {
		int x = 0;
		if(tempo%4 > 1) x = 5;
		else x = 0;
		if(n->direcao == -DELAY_GIRO) {
			n->bitmap = n->sprites[0 + x];
		} else if(n->direcao < 0) {
			n->bitmap = n->sprites[1 + x];
		} else if(n->direcao == 0) {
			n->bitmap = n->sprites[2 + x];
		} else if(n->direcao != DELAY_GIRO) {
			n->bitmap = n->sprites[3 + x];
		} else if(n->direcao == DELAY_GIRO) {
			n->bitmap = n->sprites[4 + x];
		}
	}
	
	void atualizarDisparos(Nave *n, int a) {
		if(n->delayTiro > 0) n->delayTiro--;
		if(a && !n->expl) {
			if(n->delayTiro == 0) {
				n->delayTiro = n->cadencia;
				if(n->numb != 400000)n->numb++;
				else n->numb = 1;
				n->balas1[n->numb%20]->px = n->px + n->pxb1;
				n->balas1[n->numb%20]->py = n->py + n->pyb1;
				n->balas1[n->numb%20]->pintar = 1;
				
				n->balas2[n->numb%20]->px = n->px + n->pxb2;
				n->balas2[n->numb%20]->py = n->py + n->pyb2;
				n->balas2[n->numb%20]->pintar = 1;
				
				n->balas3[n->numb%20]->px = n->px + n->pxb3;
				n->balas3[n->numb%20]->py = n->py + n->pyb3;
				n->balas3[n->numb%20]->pintar = 1;
			}
		}
		
		int i;
		for(i = 0; i < 20; i++) {
			if(n->balas1[i]->py > V_MAX_Y - n->bitbal1->h && n->balas1[i]->py < MAX_Y && 
				 n->balas1[i]->px > V_MAX_X - n->bitbal1->w && n->balas1[i]->px < MAX_X) {
				n->balas1[i]->py = n->balas1[i]->py + n->balas1[i]->vy;
				n->balas1[i]->px = n->balas1[i]->px + n->balas1[i]->vx;      
			} else {
				n->balas1[i]->pintar = 0;
			}
			if(n->balas2[i]->py > V_MAX_Y - n->bitbal2->h && n->balas2[i]->py < MAX_Y && 
				 n->balas2[i]->px > V_MAX_X - n->bitbal2->w && n->balas2[i]->px < MAX_X) {
				n->balas2[i]->py = n->balas2[i]->py + n->balas2[i]->vy;
				n->balas2[i]->px = n->balas2[i]->px + n->balas2[i]->vx;        
			} else {
				n->balas2[i]->pintar = 0;
			}
			if(n->balas3[i]->py > V_MAX_Y - n->bitbal1->h && n->balas3[i]->py < MAX_Y && 
				 n->balas3[i]->px > V_MAX_X - n->bitbal1->w && n->balas3[i]->px < MAX_X) {
				n->balas3[i]->py = n->balas3[i]->py + n->balas3[i]->vy;
				n->balas3[i]->px = n->balas3[i]->px + n->balas3[i]->vx;        
			} else {
				n->balas3[i]->pintar = 0;
			}
		}
	}
	
	void pintarExplosao(Nave * n, BITMAP * buffer) {
		if(n->expl < 10) draw_sprite(buffer, n->explosao[0], n->px + n->pxexp, n->py + n->pyexp);
		else if(n->expl < 20) draw_sprite(buffer, n->explosao[1], n->px + n->pxexp, n->py + n->pyexp);
		else if(n->expl < 30) draw_sprite(buffer, n->explosao[2], n->px + n->pxexp, n->py + n->pyexp);
		else if(n->expl < 40) draw_sprite(buffer, n->explosao[3], n->px + n->pxexp, n->py + n->pyexp);
		else if(n->expl < 50) draw_sprite(buffer, n->explosao[4], n->px + n->pxexp, n->py + n->pyexp);
		else if(n->expl < 60) draw_sprite(buffer, n->explosao[5], n->px + n->pxexp, n->py + n->pyexp);
		else if(n->expl < 70) draw_sprite(buffer, n->explosao[6], n->px + n->pxexp, n->py + n->pyexp);
		else if(n->expl < 80) draw_sprite(buffer, n->explosao[7], n->px + n->pxexp, n->py + n->pyexp);
		else if(n->expl < 90) draw_sprite(buffer, n->explosao[8], n->px + n->pxexp, n->py + n->pyexp);
		else draw_sprite(buffer, n->explosao[9], n->px + n->pxexp, n->py + n->pyexp);
		n->expl++;
	}
	
	void pintarNave(Nave * n, BITMAP * fundo) {
		if(n->pintar)draw_sprite(fundo, n->bitmap, n->px, n->py);
		if(n->expl > 0 && n->expl < 100)pintarExplosao(n, fundo);
		if(n->expl > 50) n->pintar = 0;
		int i;
		for(i = 0; i < 20; i++) {
			if(n->balas1[i]->pintar) draw_sprite(fundo, n->bitbal1, n->balas1[i]->px, n->balas1[i]->py);
			if(n->balas2[i]->pintar) draw_sprite(fundo, n->bitbal2, n->balas2[i]->px, n->balas2[i]->py);
			if(n->balas3[i]->pintar) draw_sprite(fundo, n->bitbal3, n->balas3[i]->px, n->balas3[i]->py);
		}
	}
	
	void pintarNaves(Tela * t, BITMAP * fundo) {
		int i;
		for(i = 0; i < 4; i++) {
			if(t->navesAliadas[i] != NULL) pintarNave(t->navesAliadas[i], fundo);
		}
		int j;
		for(j = 0; j < 30; j++) {
			if(t->navesInimigas[j] != NULL) pintarNave(t->navesInimigas[j], fundo);
		}
	}
	
	void setNave(Nave * n, int px, int py, int vx, int vy, int acel, int cadencia) {
		n->px = px;
		n->py = py;
		n->vx = vx;
		n->vy = vy;
		n->acel = acel;
		n->cadencia = cadencia; 
	}
	
	void setExplosaoNave(Nave * n, char * explosao, int pxexp, int pyexp, int wexp, int hexp) {
		BITMAP * exp = load_bitmap(explosao, NULL);
		int x = 0;
		int i;
		for(i = 0; i < 10; i++) {
			n->explosao[i] = create_sub_bitmap(exp, x, 0, wexp, hexp);
			x = x + wexp;
		}
		n->pxexp = pxexp;
		n->pyexp = pyexp;
		n->wexp = wexp;
		n->hexp = hexp;
	}
	
	void setDisparosNave(Nave * n, int pxb1, int pyb1, int vxb1, int vyb1, int danob1,
																 int pxb2, int pyb2, int vxb2, int vyb2, int danob2,
																 int pxb3, int pyb3, int vxb3, int vyb3, int danob3,
																 char * bal1, char * bal2, char * bal3) {
		
		BITMAP * bala1 = load_bitmap(bal1, NULL);
		BITMAP * bala2 = load_bitmap(bal2, NULL);
		BITMAP * bala3 = load_bitmap(bal3, NULL);
		
		n->bitbal1 = bala1;
		n->bitbal2 = bala2;
		n->bitbal3 = bala3;
		
		n->pxb1 = pxb1;
		n->pyb1 = pyb1;
		n->danob1 = danob1;
		n->pxb2 = pxb2;
		n->pyb2 = pyb2;
		n->danob2 = danob2;
		n->pxb3 = pxb3;
		n->pyb3 = pyb3;
		n->danob3 = danob3;
		int i;
		for(i = 0; i < 20; i++) {
			 n->balas1[i]->vx = vxb1;
			 n->balas1[i]->vy = vyb1;
			 n->balas2[i]->vx = vxb2;
			 n->balas2[i]->vy = vyb2;
			 n->balas3[i]->vx = vxb3;
			 n->balas3[i]->vy = vyb3;
		 }
	}
	
	int colisaoCaixa(BITMAP * bit1, int px1, int py1, BITMAP * bit2, int px2, int py2) {
		return !(px1 > px2 + bit2->w ||
						 py1 > py2 + bit2->h ||
						 px2 > px1 + bit1->w ||
						 py2 > py1 + bit1->h);
	}
	
	int colisaoPixelPerfect(BITMAP * bit1, int px1, int py1, BITMAP * bit2, int px2, int py2) {

		int x01 = max(px2 - px1, 0);
		int y01 = max(py2 - py1, 0);
		int xf1 = min(px2 + bit2->w - px1, bit1->w);
		int yf1 = min(py2 + bit2->h - py1, bit1->h);
		
		int x02 = max(px1 - px2, 0);
		int y02 = max(py1 - py2, 0);
		
		int j2 = y02;
		int i2 = x02;
		int j1;       
		for(j1 = y01; j1 < yf1; j1++) {
			int i1;
			for(i1 = x01; i1 < xf1; i1++) {
				if(getpixel(bit1, i1, j1) != ROSA && getpixel(bit2, i2, j2) != ROSA) return 1;
				i2++;
			}
			j2++;
			i2 = x02;
		}
		return 0;
	}
	
	int colisaoNave(BITMAP * bit1, int px1, int py1, BITMAP *bit2, int px2, int py2) {
		return colisaoCaixa(bit1, px1, py1, bit2, px2, py2) &&
					 colisaoPixelPerfect(bit1, px1, py1, bit2, px2, py2);
		
	}
	
	void inserirNaTela(Tela * t, Nave * n, int opcao) {
		int i = 0;
		while(t->navesAliadas[i] != NULL && i < 4) i++;
		int j = 0;
		while(t->navesInimigas[j] != NULL && j < 30) j++;
		if(opcao) t->navesAliadas[i] = n;
		else t->navesInimigas[j] = n;
	}
	
	void removerDestrocos(Tela * t) {
		int i;
		for(i = 0; i < 4; i++) {
			if(t->navesAliadas[i] != NULL) {
				int balasFora = 0;
				int k;
				for(k = 0; k < 20; k++) {
					if(!t->navesAliadas[i]->balas1[k]->pintar &&
						 !t->navesAliadas[i]->balas2[k]->pintar &&
						 !t->navesAliadas[i]->balas3[k]->pintar) balasFora++;
				}
				if(t->navesAliadas[i]->expl > 99 && balasFora == 20) {
						//free (t->navesAliadas[i]);
						t->navesAliadas[i] = NULL;
				}
			} 
		}int j;
		for(j = 0; j < 30; j++) {
			if(t->navesInimigas[j] != NULL) {
				int balasFora = 0;
				int k;
				for(k = 0; k < 20; k++) {
					if(!t->navesInimigas[i]->balas1[k]->pintar &&
						 !t->navesInimigas[i]->balas2[k]->pintar &&
						 !t->navesInimigas[i]->balas3[k]->pintar) balasFora++;
				}
				if(t->navesInimigas[j]->expl > 99 && balasFora == 20) {
						//free (t->navesInimigas[j]);
						t->navesInimigas[j] = NULL;
				}
			}
		}
	}
	
	void removerDaTela(Tela * t, Nave * n) {
		int i;
		for(i = 0; i < 4; i++) {
			if(t->navesAliadas[i] != NULL) {
				if(t->navesAliadas[i] == n) t->navesAliadas[i] = NULL;
			} 
		}int j;
		for(j = 0; j < 30; j++) {
			if(t->navesInimigas[j] != NULL) {
				if(t->navesInimigas[j] == n) t->navesInimigas[j] = NULL;
			}
		}
	}
	
	void colisao(Tela * tela) {
		Nave * n1;
		Nave * n2;
		int i;
		for(i = 0; i < 4; i++) {
			if(tela->navesAliadas[i] != NULL) {
				int j;
				for(j = 0; j < 30; j++) {
					if(tela->navesInimigas[j] != NULL) {
						n1 = tela->navesAliadas[i];
						n2 = tela->navesInimigas[j];
						//colisao entre naves           
						if(colisaoNave(n1->bitmap, n1->px, n1->py, n2->bitmap, n2->px, n2->py)
							 && !n1->expl && !n2->expl) {
							n1->expl = 1;
							n2->expl = 1;
						} int k;
						for(k = 0; k < 20; k++) {
							
							//colisao dos disparos
							if(n1->balas1[k] != NULL && n1->balas1[k]->pintar) {
								if(colisaoNave(n1->bitbal1, n1->balas1[k]->px, n1->balas1[k]->py, n2->bitmap, n2->px, n2->py)
									 && !n2->expl) {
									n2->expl = 1;
									n1->balas1[k]->pintar = 0;
								}
							} if(n1->balas2[k] != NULL && n1->balas2[k]->pintar) {
								if(colisaoNave(n1->bitbal2, n1->balas2[k]->px, n1->balas2[k]->py, n2->bitmap, n2->px, n2->py)
									 && !n2->expl) {
									n2->expl = 1;
									n1->balas2[k]->pintar = 0;
								}
							} if(n1->balas1[k] != NULL && n1->balas1[k]->pintar) {
								if(colisaoNave(n1->bitbal1, n1->balas1[k]->px, n1->balas1[k]->py, n2->bitmap, n2->px, n2->py)
									 && !n2->expl) {
									n2->expl = 1;
									n1->balas1[k]->pintar = 0;
								}
							}
							//tiros da nave2
							if(n2->balas1[k] != NULL && n2->balas1[k]->pintar) {
								if(colisaoNave(n2->bitbal1, n2->balas1[k]->px, n2->balas1[k]->py, n1->bitmap, n1->px, n1->py)
									 && !n1->expl) {
									n1->expl = 1;
									n2->balas1[k]->pintar = 0;
								}
							} if(n2->balas2[k] != NULL && n2->balas2[k]->pintar) {
								if(colisaoNave(n2->bitbal2, n2->balas2[k]->px, n2->balas2[k]->py, n1->bitmap, n1->px, n1->py)
									 && !n1->expl) {
									n1->expl = 1;
									n2->balas2[k]->pintar = 0;
								}
							} if(n2->balas3[k] != NULL && n2->balas3[k]->pintar) {
								if(colisaoNave(n2->bitbal3, n2->balas3[k]->px, n2->balas3[k]->py, n1->bitmap, n1->px, n1->py)
									 && !n1->expl) {
									n1->expl = 1;
									n2->balas3[k]->pintar = 0;
								}
							}
						}
					}
				}
			}
		}
	}
	
	int main() {
	 allegro_init();
	 install_keyboard();
	 set_color_depth(COLOR_BITS);
	 if (set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0) {
			//printf("Erro ao carregar o modo de vídeo!");
			return 0;
	 }
	 
	 //fundo
	 BITMAP *buffer = create_bitmap(MAX_X, MAX_Y);
	 Scroll *scroll1 = carregarScroll("rsc/fundo2.bmp", NULL);
	 Scroll *scroll2 = carregarScroll("rsc/fundo2.bmp", NULL);
	 setScroll(scroll1, -600, 1);
	 setScroll(scroll2, -1800, 1);
	 
	 Scroll *scroll3 = carregarScroll("rsc/fundo3.bmp", NULL);
	 Scroll *scroll4 = carregarScroll("rsc/fundo3.bmp", NULL);
	 setScroll(scroll3, -600, 1);
	 setScroll(scroll4, -1800, 1);
	 
	 //tempo e posicao
	 int tempo = 0;
	 int loopt = 0;
	 
	 //tela
	 Tela * tela = carregarTela();
	 
	 //criando a nave
	 Nave * nave1 = carregarNave("rsc/spritesnave2.bmp", 4);   
	 setNave(nave1, 100, 350, 3, 3, 4, 30);
	 setExplosaoNave(nave1, "rsc/explosao.bmp", -6, 3, 32, 24);
	 setDisparosNave(nave1, 0, 10, -1, -3, 7,
													16, 10, 1, -3, 7,
													0, 0, 0, 0, 0,
													"rsc/tiro.bmp", "rsc/tiro.bmp", "rsc/vazio.bmp");
	 nave1->pintar = 1;
	 inserirNaTela(tela, nave1, 1);
	 
	 Nave * nave2 = carregarNave("rsc/spritesnave2.bmp", 3);   
	 setNave(nave2, 392, 350, 3, 3, 4, 30);
	 setExplosaoNave(nave2, "rsc/explosao.bmp", -6, 3, 32, 24);
	 setDisparosNave(nave2, 2, -20, 0, -2, 7,
													0, 0, 0, 0, 0,
													0, 0, 0, 0, 0,
													"rsc/tiro2.bmp", "rsc/vazio.bmp", "rsc/vazio.bmp");
	 nave2->pintar = 1;
	 inserirNaTela(tela, nave2, 1);
	 
	 //naves inimigas
	 Nave ** navesInimigas1 = (Nave **)calloc(sizeof(Nave *), 100); 
	 int i;
	 for(i = 0; i < 100; i++) {
		 navesInimigas1[i] = carregarNave("rsc/spritesnave.bmp", 3);
		 setNave(navesInimigas1[i], 0, 0, 1, 1, 3, 100);
		 setExplosaoNave(navesInimigas1[i], "rsc/explosao.bmp", -6, 3, 32, 24);
		 setDisparosNave(navesInimigas1[i], 7, 30, 0, 1, 7,
																				0, 0, 0, 0, 0,
																				0, 0, 0, 0, 0,
																				"rsc/tiro8.bmp", "rsc/vazio.bmp", "rsc/vazio.bmp");
	 }
	 
	 Nave ** navesInimigas2 = (Nave **)calloc(sizeof(Nave *), 100);
	 for(i = 0; i < 100; i++) {
		 navesInimigas2[i] = carregarNave("rsc/spritesnave.bmp", 0);
		 setNave(navesInimigas2[i], 0, 0, 1, 1, 3, 100);
		 setExplosaoNave(navesInimigas2[i], "rsc/explosao.bmp", -6, 3, 32, 24);
		 setDisparosNave(navesInimigas2[i], 0, 15, 0, 1, 5,
																				15, 15, 0, 1, 5,
																				0, 0, 0, 0, 0,
																				"rsc/tiro8.bmp", "rsc/tiro8.bmp", "rsc/vazio.bmp");
	 }
	 
	 Nave ** navesInimigas3 = (Nave **)calloc(sizeof(Nave *), 100); 
	 for(i = 0; i < 100; i++) {
		 navesInimigas3[i] = carregarNave("rsc/spritesnave.bmp", 1);
		 setNave(navesInimigas3[i], 0, 0, 1, 1, 3, 100);
		 setExplosaoNave(navesInimigas3[i], "rsc/explosao.bmp", -6, 3, 32, 24);
		 setDisparosNave(navesInimigas3[i], -2, 15, -2, 1, 5,
																				9, 30, 0, 1, 7,
																				13, 15, 2, 1, 5,
																				"rsc/tiro8.bmp", "rsc/tiro8.bmp", "rsc/tiro8.bmp");
	 }
	 
	 //tela inicial
	 /*int inicio = 0;
	 while(!inicio) {
		 clear(buffer);
		 
		 draw_sprite(buffer, scroll4->bitmap, 0, scroll4->py);
	 }//*/
	 
	 //jogo
	 while(!key[KEY_ESC]) {	 
			
			tempo++;
			if(tempo == 100000000) {tempo = 0; loopt++;}
			clear(buffer);

			//nave aliadas
			if(!nave1->expl) {
				moverNave(nave1, key[KEY_I], key[KEY_K], key[KEY_J], key[KEY_L], tempo);
				atualizarNave(nave1, tempo);
			}
			atualizarDisparos(nave1, key[KEY_A]);
			
			if(!nave2->expl) {
				moverNave(nave2, key[KEY_UP], key[KEY_DOWN], key[KEY_LEFT], key[KEY_RIGHT], tempo);
				atualizarNave(nave2, tempo);
			}
			atualizarDisparos(nave2, key[KEY_INSERT]);
			
			
			
			//naves inimigas
			
			//ataques
			if(tempo == 500) {
				int i;
				for(i = 0; i < 7; i++) {
					inserirNaTela(tela, navesInimigas1[i], 0);
					navesInimigas1[i]->px = (i + 1) * 60;
					if(i < 4)navesInimigas1[i]->py = -50 * (i + 1);
					else navesInimigas1[i]->py = -250 + 50 * (i - 2);
					navesInimigas1[i]->pintar = 1;
				}
			} for(i = 0; i < 7; i++) {
				if(!navesInimigas1[i]->expl) {
					moverNaveInimiga(navesInimigas1[i], 0, 1, 0, 0, tempo);
					atualizarNave(navesInimigas1[i], tempo);       
				}
				atualizarDisparos(navesInimigas1[i], 1);
			}
			
			if(tempo == 900) {
				int i;
				for(i = 14; i < 22; i++) {
					inserirNaTela(tela, navesInimigas1[i], 0);
					if(i < 17) {
						navesInimigas1[i]->px = -30 -(i - 14) * 10;
						navesInimigas1[i]->py = 60 * (i - 15) ;
					} else {
						navesInimigas1[i]->px = 542 + (i - 17) * 10;
						navesInimigas1[i]->py = 60 * (i - 18);
					}
					navesInimigas1[i]->pintar = 1;
				}
			} for(i = 14; i < 22; i++) {
				if(!navesInimigas1[i]->expl) {
					if(i < 17) moverNaveInimiga(navesInimigas1[i], 0, 1, 0, 1, tempo);
					else moverNaveInimiga(navesInimigas1[i], 0, 1, 1, 0, tempo);
					atualizarNave(navesInimigas1[i], tempo);
				}
				atualizarDisparos(navesInimigas1[i], 1);
			}
			
			if(tempo == 1400) {
				int i;
				for(i = 7; i < 14; i++) {
					inserirNaTela(tela, navesInimigas1[i], 0);
					navesInimigas1[i]->px = (i - 6) * 60;
					navesInimigas1[i]->py = 50 * abs(i - 10);
					navesInimigas1[i]->py = -navesInimigas1[i]->py;
					navesInimigas1[i]->pintar = 1;
				}
			} for(i = 7; i < 14; i++) {
				if(!navesInimigas1[i]->expl) {
					moverNaveInimiga(navesInimigas1[i], 0, 1, 0, 0, tempo);
					atualizarNave(navesInimigas1[i], tempo);       
				}
				atualizarDisparos(navesInimigas1[i], 1);
			}
			
			if(tempo == 1700) {
				int i;
				for(i = 0; i < 6; i++) {
					inserirNaTela(tela, navesInimigas3[i], 0);
					if(i < 3) navesInimigas3[i]->px = 60;
					else navesInimigas3[i]->px = 452;
					if(i < 3) navesInimigas3[i]->py = -50 * (i + 1);
					else navesInimigas3[i]->py = -50 * (i - 2);
					navesInimigas3[i]->pintar = 1;
				}
			} for(i = 0; i < 6; i++) {
				if(!navesInimigas3[i]->expl) {
					if(i < 3)moverNaveInimiga(navesInimigas3[i], 0, 1, 0, 1, tempo);
					else moverNaveInimiga(navesInimigas3[i], 0, 1, 1, 0, tempo);
					atualizarNave(navesInimigas3[i], tempo);       
				}
				atualizarDisparos(navesInimigas3[i], 1);
			}
			
			if(tempo == 2000) {
				int i;
				for(i = 6; i < 9; i++) {
					inserirNaTela(tela, navesInimigas3[i], 0);
					navesInimigas3[i]->px = -150;
					navesInimigas3[i]->py = -50 * (i - 5);
					navesInimigas3[i]->pintar = 1;
				}
			} for(i = 6; i < 9; i++) {
				if(!navesInimigas3[i]->expl) {
					moverNaveInimiga(navesInimigas3[i], 0, 1, 0, 1, tempo);
					atualizarNave(navesInimigas3[i], tempo);       
				}
				atualizarDisparos(navesInimigas3[i], 1);
			}
			
			if(tempo == 2100) {
				int i;
				for(i = 14; i < 22; i++) {
					inserirNaTela(tela, navesInimigas1[i], 0);
					navesInimigas1[i]->px = 247;
					navesInimigas1[i]->py = -50 * (i - 13);
					navesInimigas1[i]->pintar = 1;
				}
			} for(i = 14; i < 22; i++) {
				if(!navesInimigas1[i]->expl) {
					moverNaveInimiga(navesInimigas1[i], 0, 1, 0, 0, tempo);
					atualizarNave(navesInimigas1[i], tempo);       
				}
				atualizarDisparos(navesInimigas1[i], 1);
			}
			
			if(tempo == 2200) {
				int i;
				for(i = 9; i < 12; i++) {
					inserirNaTela(tela, navesInimigas3[i], 0);
					navesInimigas3[i]->px = 662;
					navesInimigas3[i]->py = -50 * (i - 8);
					navesInimigas3[i]->pintar = 1;
				}
			} for(i = 9; i < 12; i++) {
				if(!navesInimigas3[i]->expl) {
					moverNaveInimiga(navesInimigas3[i], 0, 1, 1, 0, tempo);
					atualizarNave(navesInimigas3[i], tempo);       
				}
				atualizarDisparos(navesInimigas3[i], 1);
			}
			

			//scrolling         
			moverScroll(scroll1, scroll2, 2, tempo);
			moverScroll(scroll3, scroll4, 1, tempo);
			
			//colisao
			colisao(tela);
			removerDestrocos(tela);
			
			//pintura dos sprites na tela
			draw_sprite(buffer, scroll1->bitmap, 0, scroll1->py);
			draw_sprite(buffer, scroll2->bitmap, 0, scroll2->py);
			draw_sprite(buffer, scroll3->bitmap, 0, scroll3->py);
			draw_sprite(buffer, scroll4->bitmap, 0, scroll4->py);
			
			pintarNaves(tela, buffer);
			draw_sprite(screen, buffer, 0, 0);
			
	 }
		 
	 allegro_exit();
 return 0;
 }END_OF_MAIN();