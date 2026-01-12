FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

# Installazione strumenti completa per Tesi Matematica
RUN apt-get update && apt-get install -y \
    texlive-latex-base \
    texlive-latex-recommended \
    texlive-latex-extra \
    texlive-fonts-recommended \
    texlive-fonts-extra \
    texlive-lang-italian \
    chktex \
    aspell \
    aspell-it \
    g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY tester.cpp .
# Compiliamo l'engine e lo mettiamo nella radice / (fuori da /app)
RUN g++ -O3 tester.cpp -o /engine

WORKDIR /app
# Eseguiamo l'engine puntando al percorso assoluto
CMD ["/engine"]