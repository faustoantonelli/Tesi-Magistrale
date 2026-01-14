FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    texlive-latex-extra \
    texlive-lang-italian \
    chktex \
    aspell \
    aspell-it \
    g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY tester.cpp .
RUN g++ -O3 tester.cpp -o /engine
# Rendiamo il binario eseguibile ovunque
RUN chmod +x /engine

WORKDIR /app
CMD ["/engine"]