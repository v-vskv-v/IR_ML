import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileUtil;

public class CpFile
{
    public static void main(String[] args) throws Exception
    {
        if (args.length != 2)
        {
            System.out.println("Bad args.");
            return;
        }
        Path path1 = new Path(args[0]);
        Path path2 = new Path(args[1]);
        Configuration conf = new Configuration();
        FileSystem fs1 = path1.getFileSystem(conf);
        FileSystem fs2 = path2.getFileSystem(conf);
        FileUtil.copy(fs1, path1, fs2, path2, false, conf);
    }
}
