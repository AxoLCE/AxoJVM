package axo.jvm.api;

import axo.jvm.helpers.ModRegistry;

import java.time.LocalTime;
import java.time.format.DateTimeFormatter;

public class Log {
    private static final DateTimeFormatter TIME_FORMAT = DateTimeFormatter.ofPattern("HH:mm:ss");
    private static final StackWalker WALKER = StackWalker.getInstance(StackWalker.Option.RETAIN_CLASS_REFERENCE);

    private static void log(String level, String message, boolean isError){
        String time = LocalTime.now().format(TIME_FORMAT);
        Class<?> callerClass = WALKER.walk(stream -> stream
                .map(StackWalker.StackFrame::getDeclaringClass)
                .filter(clazz -> clazz != Log.class)
                .findFirst()
                .orElse(Log.class));
        String callerClassName = callerClass.getName();
        String modId = ModRegistry.getModId(callerClassName);
        String fullMessage = String.format("[%s] ["+modId+"/%s]: %s", time, level, message);
        if(isError){
            System.err.println(fullMessage);
        }else {
            System.out.println(fullMessage);
        }
    }

    public static void info(String message){
        log("[INFO]", message, false);
    }
    public static void warn(String message){
        log("[WARN]", message, false);
    }
    public static void error(String message){
        log("[ERROR]", message, true);
    }

    public static void error(String message, Throwable throwable){
        log("[ERROR]", message, true);
        if (throwable != null){
            throwable.printStackTrace();
        }
    }
}
