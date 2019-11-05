FROM gcc:latest

RUN apt update && apt -y install build-essential cmake && rm -rf /var/lib/apt/lists/*
RUN apt update && apt -y install build-essential make && rm -rf /var/lib/apt/lists/*

#RUN apt-get update && apt-get install -y nmap vim

COPY . /usr/src/myapp
WORKDIR /usr/src/myapp

RUN cmake . && make
CMD ["./Asyncpp"]

