FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

# Installazione strumenti
RUN apt-get update && apt-get install -y \
    texlive-latex-base \
    texlive-latex-extra \
    texlive-lang-italian \
    texlive-fonts-recommended \
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