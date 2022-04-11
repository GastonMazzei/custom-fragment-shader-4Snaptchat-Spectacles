# We simply inherit the Python 3 image. This image does
# not particularly care what OS runs underneath
FROM python:3.6

# Set an environment variable with the directory
# where we'll be running the app
ENV APP /app-custom-spectacles-shader

# Create the directory and instruct Docker to operate
# from there from now on
RUN mkdir $APP
WORKDIR $APP

# Expose the port uWSGI will listen on
EXPOSE 5004

# Copy the requirements file in order to install
# Python dependencies
COPY requirements-custom-spectacles-shader.txt .

# Install Python dependencies
RUN pip3 install --upgrade pip
RUN pip3 install -r requirements-custom-spectacles-shader.txt

# We copy the rest of the codebase into the image
COPY . .

# Finally, we run uWSGI with the ini file we
# created earlier
#COPY docker-command.sh .
#RUN chmod +x docker-command.sh
#RUN docker-command.sh
#CMD [ "uwsgi", "--ini", "app.ini" ]
CMD [ "/bin/bash", "docker-command.sh" ]
