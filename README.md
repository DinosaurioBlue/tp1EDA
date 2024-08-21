# tp1EDA

SIMULACION DE SISTEMA SOLAR O ALPHA CENTAURI
El programa fue realizado de manera que segun si de define la macro SISTEMA_SOLAR o ALPHA_CENTAURI (no esta definido el uso de las dos al mismo tiempo) la simulacion muestra ese respectivo sistema.

DECISIONES DE OPTIMIZACION: cuellos de botella.
- Calculos:
    Preservando la fluidez de la simulacion, y teniendo en cuenta que la masa de los asteroides es despreciable en comparacion a la de los cuerpos celestes mas masivos, realizamos los calculos de manera que la masa de estos ultimos influya en la de los asteroides, pero no al reves. Para mas informacion acerca de los algoritmos usados y su O grande, revisar calculos.cpp.
- Grafico:
    Una de las mayores perdidas de fotogramas se produce al graficar los cuerpos celestes como esferas. Para solucionarlo tuvimos en cuenta que los asteorides son los cuerpos celestes con menos masa en la simulacion, y por lo tanto, a cierta distancia pueden ser graficados como puntos sin perder demasiado realismo. Por lo tanto, en renderView tomamos en cuenta la distancia entre la camara y cada asteroide para ver si lo graficamos como un punto o una esfera.




Debido a que el tamano de los asteroides es despreciable en comparacion a los cuerpos celestes, se tomo la decision de renderizarlos con la funcion DrawPoint, ya que asi consume menos recursos.