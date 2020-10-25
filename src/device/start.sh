#!/bin/bash

CROSS_COMPILE="OFF"

clean() 
{
	rm -rf $TOP_DIR/build/*
	rm -rf $TOP_DIR/include/*
	rm -rf $TOP_DIR/lib/*
	rm -rf $TOP_DIR/bin/*
	rm -rf $TOP_DIR/share/*
	echo "clean ok."
}

compile_sdk() 
{
	echo "compiling samurai sdk..."
	cd $TOP_DIR/build
	
	if [ ! -d $TOP_DIR/build/sdk ]; then
		mkdir -p $TOP_DIR/build/sdk
	fi
	
	cd $TOP_DIR/build/sdk
	
	CMAKE_PARAM="${TOP_DIR}/sdk -DCMAKE_INSTALL_PREFIX=${TOP_DIR}"
	if [ $CROSS_COMPILE = 'ON' ]; then
		CMAKE_PARAM="${CMAKE_PARAM} -DCROSS_COMPILE=ON -DTARGET_HOST=${TARGET_HOST} -DSYSTEM_NAME=${SYSTEM_NAME}"
	fi
	
	if [ $CURL_COMPILE = 'ON' ]; then
		CMAKE_PARAM="${CMAKE_PARAM} -DUSE_SYS_CURL=OFF"
		if [ ! -f $TOP_DIR/lib/libcurl.so ]; then
			echo "The curl is required to compile the SDK, so compile the curl first."
			compile_curl
		fi
	else
		CMAKE_PARAM="${CMAKE_PARAM} -DUSE_SYS_CURL=ON"
	fi
	
	echo "CMAKE_PARAM="$CMAKE_PARAM
	
	cmake $CMAKE_PARAM
	make 
	if [ ! -f $TOP_DIR/build/sdk/libsamurai.so ]; then
		echo "compile samurai sdk fail."
		exit 1
	fi
	
	make install
	echo "compile samurai sdk ok."
}


compile_demo() 
{
	echo "compiling exam demo..."
	cd $TOP_DIR/build
	if [ ! -d $TOP_DIR/build/app ]; then
		mkdir -p $TOP_DIR/build/app
	fi
	
	cd $TOP_DIR/build/app
	
	CMAKE_PARAM="${TOP_DIR}/app -DCMAKE_INSTALL_PREFIX=${TOP_DIR}"

	cmake $CMAKE_PARAM
	make
	if [ ! -f $TOP_DIR/build/app/exam_demo ]; then
		echo "compile exam demo fail."
		exit 1
	fi
	make install
	echo "compile exam demo ok."
}

compile_server() 
{
	echo "compiling exam server..."
	cd $TOP_DIR/build
	if [ ! -d $TOP_DIR/build/server ]; then
		mkdir -p $TOP_DIR/build/server
	fi
	
	cd $TOP_DIR/build/server
	
	CMAKE_PARAM="${TOP_DIR}/server -DCMAKE_INSTALL_PREFIX=${TOP_DIR}"

	cmake $CMAKE_PARAM
	make
	if [ ! -f $TOP_DIR/build/server/server ]; then
		echo "compile exam demo fail."
		exit 1
	fi
	make install
	echo "compile exam demo ok."
}

compile_curl()
{
	CURL_CONFIG="--prefix=$TOP_DIR/ \
				 --enable-shared \
				 --enable-static \
				 --without-libidn \
				 --without-librtmp \
				 --without-nss \
				 --without-libssh2 \
				 --without-zlib \
				 --without-winidn \
				 --disable-rtsp \
				 --disable-ldap \
				 --disable-ldaps \
				 --disable-ipv6 \
				 --disable-ftp \
				 --disable-proxy \
				 --disable-telnet \
				 --disable-tftp \
				 --disable-pop3 \
				 --disable-imap \
				 --disable-smb \
				 --disable-smtp \
				 --disable-gopher  \
				 --disable-dict  \
				 --disable-file" 
	if [ $CROSS_COMPILE = 'ON' ]; then
		CONFIG_CROSS="--host=$TARGET_HOST"
		CURL_CONFIG="$CURL_CONFIG $CONFIG_CROSS"
	fi
	echo $CURL_CONFIG
	echo "compiling curl..."
	cd $TOP_DIR/build
	if [ ! -d $TOP_DIR/build/curl ]; then
		mkdir -p $TOP_DIR/build/curl
		tar -xzf $TOP_DIR/vendor/curl-7.72.0.tar.gz -C curl --strip-components 1
	fi
	cd $TOP_DIR/build/curl
	./configure $CURL_CONFIG
	
	make
	if [ ! -f $TOP_DIR/build/curl/lib/.libs/libcurl.so ]; then
		echo "compile curl fail."
		exit 1
	fi
	make install
	echo "compile curl ok."
	
	rm -rf $TOP_DIR/lib/libcurl.so.4
}

compile_mqtt()
{
	echo "compiling mqtt..."
	cd $TOP_DIR/build
	if [ ! -d $TOP_DIR/build/mqtt ]; then
		mkdir -p $TOP_DIR/build/mqtt
		#tar -xzf $TOP_DIR/vendor/mqtt.tar.gz -C mqtt --strip-components 1
	fi
	cd $TOP_DIR/build/mqtt
	if [ $CROSS_COMPILE = 'ON' ]; then
		cmake $TOP_DIR/vendor/mqtt -DCMAKE_INSTALL_PREFIX=$TOP_DIR -DCROSS_COMPILE=ON -DTARGET_HOST=$TARGET_HOST -DSYSTEM_NAME=$SYSTEM_NAME
	else
		cmake $TOP_DIR/vendor/mqtt -DCMAKE_INSTALL_PREFIX=$TOP_DIR
	fi
	make
	if [ ! -f $TOP_DIR/build/mqtt/libmqtt.so ]; then
		echo "compile mqtt fail."
		exit 1
	fi
	make install
	echo "compile mqtt ok."
}

compile_vendor()
{
	compile_curl 
	compile_mqtt 
}

compile_all()
{
	compile_vendor
	compile_sdk 
	compile_demo
	
}

main()
{	
	if [ ! -d build ]; then
		mkdir build
	fi

	case $1 in
		clean) 
			clean 
		;; 
		sdk) 
			compile_sdk 
		;; 
		demo) 
			compile_demo 
		;; 
		curl)
			compile_curl
		;;
		mqtt)
			compile_mqtt
		;;
		vendor)
			compile_vendor 
		;;
		server) 
			compile_server 
		;;
		*)
			compile_all
		;;
	esac
	
	# if [ ! $1 ]; then
		# /sbin/ldconfig $TOP_DIR/lib
	# fi

	# if [ ! $1 = 'clean' ]; then
		# /sbin/ldconfig $TOP_DIR/lib
	# fi
}

main $1
#rm -rf $TOP_DIR/lib/libcurl.so.4

