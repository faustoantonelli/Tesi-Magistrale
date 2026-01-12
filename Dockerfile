FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    texlive-latex-extra \
    texlive-lang-italian \
    texlive-fonts-recommended \
    chktex \
    aspell \
    aspell-it \
    g++ \
    grep \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY tester.cpp .
RUN g++ tester.cpp -o engine

CMD ["./engine"]