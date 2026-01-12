# Studio del Rango di Curve Ellittiche e Gruppi di Selmer

![Matematica](https://img.shields.io/badge/Area-Algebraic%20Number%20Theory-blue)
![LaTeX](https://img.shields.io/badge/Tools-LaTeX-008080?style=flat&logo=latex&logoColor=white)
![Strumenti](https://img.shields.io/badge/Tools-SageMath%20%7C%20LaTeX-green)
![MATLAB](https://img.shields.io/badge/Tools-MATLAB-ED8B00?style=flat&logo=matlab&logoColor=white)

## 📌 Abstract del Progetto
Questa repository contiene il materiale di ricerca e il supporto computazionale relativo alla tesi riguardante lo studio del **rango** di una curva ellittica $E$ definita su un campo di numeri $K$. Il lavoro si concentra sull'analisi della discesa (n-descent) e sull'utilizzo dei **Gruppi di Selmer** $S^{(n)}(E/K)$ per determinare un limite superiore al rango del gruppo di Mordell-Weil $E(K)$.
Il problema centrale della tesi è determinare la struttura di $E(K) \cong E(K)_{tors} \times \mathbb{Z}^r$. Poiché il calcolo del rango $r$ è un problema aperto e non algoritmico in senso generale, ci si avvale della successione esatta fondamentale:

$$0 \to E(K)/nE(K) \to S^{(n)}(E/K) \to \text{Ш}(E/K)[n] \to 0$$

Dove:
* **$S^{(n)}(E/K)$** è il gruppo di Selmer, che è computabile localmente.
* **$\text{Ш}(E/K)$** è il gruppo di Shafarevich-Tate, che contiene le ostruzioni al principio locale-globale.
