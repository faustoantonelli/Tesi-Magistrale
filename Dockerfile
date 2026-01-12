FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

# Installazione completa per tesi di Matematica
RUN apt-get update && apt-get install -y \
    texlive-latex-base \
    texlive-latex-recommended \
    texlive-latex-extra \
    texlive-fonts-recommended \
    texlive-fonts-extra \
    texlive-science \
    texlive-lang-italian \
    chktex \
    aspell \
    aspell-it \
    g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY tester.cpp .
RUN g++ -O3 tester.cpp -o /engine

WORKDIR /app
CMD ["/engine"]