
---

## Introduccion ##

Hay muchos tipos de ondas sismicas pero en la practica se distinguen 4 tipos:

Ondas Compresionales P (o primarias).
Ondas Transversales S (o secundarias).
Love
Raleigh

A mayor distancia la amplitud de las ondas sismicas disminuye a medida que la energia de el evento es liberada en el viaje. A medida que tambien nos alejamos del evento los distintos tipos de ondas sismicas son separadas pues estas viajan a distintas velocidades.

Los tipos de ondas sismicas por un numero de propiedades que incluyen, la velocidad a que estas viajan, la direccion en que se mueven las particulas por las que estas pasan, y en donde, y donde no se propagan.

Las ondas sismicas se clasifican en "body waves", pues estas viajan hacia el interior de la tierra, y "surface waves" que viajan en la superficie.

En las "body waves" se distinguen 2 tipos de ondas, las ondas P, y las ondas S.

### Velocidad de las Ondas Sismicas. ###

Las ondas sismicas viajan rapido, a velocidades dentro de los kilometros por segundo. La velocidad precisa de la onda sismica depende de varios factores, donde el mas importante es la composicion de la roca. Gracias a esto, es posible tener una idea de la composicion o el conjunto de composiciones de el planeta.

Es importante notar, que este proceso no siempre es facil, pues tambien ocurre que distintos tipos de roca pueden poseer la misma velocidad de propagacion de onda sismica. Ademas otros factores pueden afectar la velocidad de propagacion de onda, en particular, la temperatura y la presion. En general, la temperatura tiende a disminuir la velocidad, de las ondas sismicas, mientras que la presion tiende a aumentar su velocidad. El comportamiento que usualmente se ve, es que las ondas sismicas tienden a aumentar velocidad a medida que estas viajan hacia el interior de la tierra por la presion aunque, la temperatura va a aumentando y reduce su velocidad en una proporcion menor.

Debemos tener en mente que la velocidad de las ondas sismicas dependeran de la composicion, la temperatura y la presion.

### Ondas Compresionales P (o Primarias) ###

Las ondas P son las primeras ondas en llegar, pues estas son las que mas rapido viajan, su nombre de hecho es por el echo de ser las primeras en llegar.  Sus velocidades, tipicamente varian entre ~1 y ~14 km/seg. Los valores mas lentos generalmente corresponden, a sus velocidades a medida que viajan en el agua, y las velocidades mas altas se obtienen a medida que estas se aproximan a la base del manto de la tierra.

La velocidad de la onda depende de las propiedades elasticas y densidad del material. Si dejamos que k nos represente "bulk modulus" de un material, u el "shear-modulus", y rho la densidad, entonces la velocidad de la onda P , que representamos con alpha, se define como:

alpha = sqrt((k + (4/3)u)/p)

El modulus nos permite medir que tan facil o dificil es deformar el material. Por ejemplo, el "bulk modulus" es una medida de como el material cambia volumen cuando la presion es aplicada y es una caracteristica del material. Por ejemplo, el caucho tiene menor "bulk modulus" que el acero.

Las ondas P, son ondas de sonido, solo que en sismica estamos interesados en ondas que estan por debajo del rango del oido humano. La vibracion causada por ondas P es un cambio en el volumen, alternando compresion a expansion en la direccion en que la onda esta viajando. Las ondas P viajan a traves de todo tipo de medios incluyendo solidos, liquidos, o gas.


### Ondas Compresionals S ( o Secundarias ) ###
Las ondas S son ondas mas lentas que las P, pero estas no cambian el volumen del material en el que se propagan, y ademas vibran el material en la direccion "transversa" o perpendicular a la direccion de propagacion.

### Ondas en un Sismograma. ###

Las diferencia de velocidades de las ondas tiene una profunda influencia en la naturaleza de los sismogramas. Pues el tiempo que demora una onda en viajar es igual a la velocidad a la que la onda ha viajado dividido por la velocidad promedio de la onda cuando se movie durante su transmision. Se espera que las ondas mas rapidas deben llegar primero.

Existen varios tipos de interacciones entre las ondas y las estructuras subterraneas de la tierra, que son comunmente observables en sismogramas:

Refraccion
Refleccion
Dispersion
Difraccion
Atenuacion

Observaremos las mas simples:

Refraccion
Cuando una onda viaja a traves de la tierra, el camino que est toma depende de la velocidad. El principio de Snell nos ayuda a identificar que camino toma la onda cuando esta es transferida de una capa terrestre a otra. El cambio de direccion depende de la razon entre las velocidades de propagacion de los distintos tipos de rocas. La refraccion afecta las ondas que viajan al interior de la tierra de una forma importante.  En general, la velocidad sismica en la tierra aumenta con la profundidad

Refleccion.
En la sismologia las reflecciones son utilizadas para encontrar petroleo e investigar la estructura interna de la tierra. En algunas instancias, las reflecciones de el limite entre el manto y la corteza.
Una refleccion sismica ocurre cuando una onda incide en un cambio en el tipo de roca. parte de la energia llevada en el incidente se transmite a por el material con un cambio de velocidad, y parte de esta es reflejada en el mismo medio de donde incidio.

## Objetivos ##

La idea general del proyecto es construir una interfaz de usurio que permita facilitar la usabilidad de un tipo de simulacion realizable en el paquete Seismic Unix.

Hay que buscar formas de mejorar la interpretacion de las simulaciones que se pueden realizar con el paquete Seismic Unix.

## Razon de este Proyecto ##
Pienso que este es un buen proyecto, ya que esta relacionado en un asunto comun. Tenemos un conjunto de programas que funcionan por linea de comando, pero requieren que el usuario final tenga alguna clase de experiencia con la programacion de scripts para utilizarlo.  Se esta introduciendo una GUI para ayudar a reducir este requerimiento sobre el usuario. Este caso es frecuente, en particular en la computacion en campos cientificos, pues producir una experiencia solida al usuario toma mucho tiempo, y este es un problema para la mayoria de los proyectos cientificos. Ya que los proyectos de software cientificos estan es enfocados inicialmente en la funcionalidad (funciona? es correcto?), no en la forma en que (como es que el usuario va a trabajar con esto). Forma vs Funcionalidad.