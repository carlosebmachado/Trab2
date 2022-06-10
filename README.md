# Graphic Computing 2 - Obj e Iluminação

## Comandos

### Rotação
- Teclas 7 e 9: rotação no eixo X
- Teclas 4 e 6: rotação no eixo Y
- Teclas 7 e 9: rotação no eixo Z
- Tecla 5: rotação aleatória em todos os eixos

### Movimentação
- Teclas direcionais CIMA, BAIXO, DIREITA E ESQUERDA: movimentação do objeto pela tela nos eixos X e Y

### Escala
- Teclas Page Up e Page Down: aumenta e diminui o objeto

### Iluminação
case 'q':
		// Ambiente
		light1 = !light1;
		break;
	case 'w':
		// Difusa
		light2 = !light2;
		break;
	case 'e':
		// Especular
