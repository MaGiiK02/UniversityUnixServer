FROM gcc:4.9
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN gcc -Wall -o myapp main.c
CMD ["./myapp"]