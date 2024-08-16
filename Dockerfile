FROM ubuntu
COPY ./build app
WORKDIR app
EXPOSE 8010
USER root
CMD ["./prodmozg2"]

