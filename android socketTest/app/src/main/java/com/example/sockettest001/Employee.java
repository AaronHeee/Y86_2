package com.example.sockettest001;
import java.io.BufferedWriter;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
/**
 * 与c语言通信(java做client，c/c++做server，传送一个结构)
 * 
 * @author kingfish
 * @version 1.0
 */


public class Employee {
	private PrintWriter out = null; 
	private byte[] buf = new byte[28]; // 为说明问题，定死大小，事件中可以灵活处理

	/**
	 * 将int转为低字节在前，高字节在后的byte数组
	 */
	private static byte[] tolh(int n) {
		byte[] b = new byte[4];
		b[0] = (byte) (n & 0xff);
		b[1] = (byte) (n >> 8 & 0xff);
		b[2] = (byte) (n >> 16 & 0xff);
		b[3] = (byte) (n >> 24 & 0xff);
		return b;
	}
	/**
	 * 将byte数组转化成String
	 */
	private static String toStr(byte[] valArr,int maxLen) {
		int index = 0;
		while(index < valArr.length && index < maxLen) {
			if(valArr[index] == 0) {
				break;
			}
			index++;
		}
		byte[] temp = new byte[index];
		System.arraycopy(valArr, 0, temp, 0, index);
		return new String(temp);
	}
	
	/**
	 * 将低字节在前高字节在后的byte数组转为int，
	 */
	private static int vtolh(byte[] bArr) {
		int n = 0;
		for(int i=0;i<bArr.length&&i<4;i++){
			int left = i*8;
			n+= (bArr[i] << left);
		}
		return n;
	}
	public String name = "";
	public int id = 0;
	public float salary = 0;
	/**
	 * 将float转为低字节在前，高字节在后的byte数组
	 */
	private static byte[] tolh(float f) {
		return tolh(Float.floatToRawIntBits(f));
	}
	
	public static Employee getEmployee(byte[] bArr) {
		String name = "";
		int id = 0;
		float salary = 0;
		
		byte[] temp = new byte[20];
		name = toStr(bArr,20);
		
		System.arraycopy(bArr, 20, temp, 0, 4);
		id = vtolh(temp);
		
		return new Employee(name, id, salary);
		

	}
	/**
	 * 构造并转换
	 */
	public Employee(String name, int id, float salary) {
		this.name = name;
		this.id = id;
		this.salary = salary;
		
		byte[] temp = name.getBytes();
		System.arraycopy(temp, 0, buf, 0, temp.length);

		temp = tolh(id);
		System.arraycopy(temp, 0, buf, 20, temp.length);

		temp = tolh(salary);
		System.arraycopy(temp, 0, buf, 24, temp.length);
	}
	
	/**
	 * 返回要发送的数组
	 */
	public byte[] getbuf() {
		return buf;
	}

	/**
	 * 发送测试
	 */
	
} // end 